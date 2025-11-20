// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_ComboAttack.h"
#include "Character/P4CharacterPlayer.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Animation/Data/P4ComboAttackData.h"

UP4GA_ComboAttack::UP4GA_ComboAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    // 부모 클래스의 태그 검증 먼저 (블루프린트에서 설정한 태그들 체크)
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_ComboAttack] Super::CanActivateAbility 실패 - 태그 차단됨"));
        return false;
    }

    // 추가 검증: 무기가 장착되어 있는지 확인
    const AP4CharacterPlayer* Player = Cast<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());
    if (!Player)
    {
        return false;
    }

    USkeletalMeshComponent* MeshComp = Player->GetMesh();
    const UP4PlayerAnimInstance* AnimInst = MeshComp ? Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;

    if (!AnimInst)
    {
        return false;
    }

    // 무기가 장착되어 있는지 확인
    if (!AnimInst->CharacterState.bIsEquipped)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GA_Draw] 무기가 장착되지 않았습니다!"));
        return false;
    }

    // 손에 칼이 없으면 공격 불가
    UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
    if (ASC)
    {
        FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
        if (!ASC->HasMatchingGameplayTag(DrawnTag))
        {
            UE_LOG(LogTemp, Warning, TEXT("[GA_ComboAttack] 손에 칼이 없습니다!"));
            return false;
        }
    }

    return true;
}

void UP4GA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AP4CharacterPlayer* P4Character = CastChecked<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());
    CurrentComboData = P4Character->GetComboAttackData();
    P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


    UAnimMontage* StrongAttackMontage = P4Character->GetComboAttackMontage();
    if (!StrongAttackMontage)
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_ComboAttack] StrongAttack 몽타주가 설정되지 않았습니다!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 몽타주 재생
    UAbilityTask_PlayMontageAndWait* PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            TEXT("ComboAttack"),
            StrongAttackMontage,
            1.0f,
            GetNextSection()
        );
    PlayMontageTask->OnCompleted.AddDynamic(this, &UP4GA_ComboAttack::OnCompleteCallback);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &UP4GA_ComboAttack::OnInterruptedCallback);

    // AT의 함수 호출
    PlayMontageTask->ReadyForActivation();
    StartComboTimer();
}

void UP4GA_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboInput = false;
    }
    else
    {
        HasNextComboInput = true;
    }
}

void UP4GA_ComboAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
    P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    CurrentComboData = nullptr;
    CurrentCombo = 0;
    HasNextComboInput = false;
}

void UP4GA_ComboAttack::OnCompleteCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UP4GA_ComboAttack::OnInterruptedCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UP4GA_ComboAttack::GetNextSection()
{
    CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
    FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
    return NextSection;
}

void UP4GA_ComboAttack::StartComboTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FRameRate;
    if (ComboEffectiveTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UP4GA_ComboAttack::CheckComboInput, ComboEffectiveTime, false);
    }
}

void UP4GA_ComboAttack::CheckComboInput()
{
    ComboTimerHandle.Invalidate();

    if (HasNextComboInput)
    {
        MontageJumpToSection((GetNextSection()));
        StartComboTimer();
        HasNextComboInput = false;
    }
}
