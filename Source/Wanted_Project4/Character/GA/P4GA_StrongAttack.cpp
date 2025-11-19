// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_StrongAttack.h"
#include "Character/P4CharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P4GA_Jump.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Character/P4CharacterPlayer.h"

UP4GA_StrongAttack::UP4GA_StrongAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_StrongAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    // 부모 클래스의 태그 검증 먼저 (블루프린트에서 설정한 태그들 체크)
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_StrongAttack] Super::CanActivateAbility 실패 - 태그 차단됨"));
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
            UE_LOG(LogTemp, Warning, TEXT("[GA_StrongAttack] 손에 칼이 없습니다!"));
            return false;
        }
    }

    return true;
}

void UP4GA_StrongAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


	UAnimMontage* StrongAttackMontage = P4Character->GetStrongAttackMontage();
    if (!StrongAttackMontage)
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_StrongAttack] StrongAttack 몽타주가 설정되지 않았습니다!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 몽타주 재생
    UAbilityTask_PlayMontageAndWait* PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            TEXT("StrongAttack"),
            StrongAttackMontage
        );
    PlayMontageTask->OnCompleted.AddDynamic(this, &UP4GA_StrongAttack::OnCompleteCallback);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &UP4GA_StrongAttack::OnInterruptedCallback);

    // AT의 함수 호출
    PlayMontageTask->ReadyForActivation();
	
}

void UP4GA_StrongAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_StrongAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
    P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UP4GA_StrongAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UP4GA_StrongAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
