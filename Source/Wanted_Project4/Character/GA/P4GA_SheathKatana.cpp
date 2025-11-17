// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_SheathKatana.h"
#include "Character/P4CharacterPlayer.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

UP4GA_SheathKatana::UP4GA_SheathKatana()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_SheathKatana::CanActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags,
    OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    // 부모 클래스의 태그 검증 먼저 (블루프린트에서 설정한 태그들 체크)
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
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
    if (!AnimInst->bIsEquipped)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GA_SheathKatana] 무기가 장착되지 않았습니다!"));
        return false;
    }

    // 손에 쥐고 있지 않으면 납도 불가
    if (!AnimInst->bIsKatanaOnHand)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GA_SheathKatana] 이미 납도 상태입니다!"));
        return false;
    }

    return true;
}

void UP4GA_SheathKatana::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AP4CharacterPlayer* Player = CastChecked<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());
    USkeletalMeshComponent* MeshComp = Player->GetMesh();
    UP4PlayerAnimInstance* AnimInst = MeshComp ? Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;

    if (!AnimInst)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UAnimMontage* SheathMontage = Player->GetSheathKatanaMontage();
    if (!SheathMontage)
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_Sheath] SheathKatana 몽타주가 설정되지 않았습니다!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 납도 상태로 변경
    AnimInst->bIsKatanaOnHand = false;

    // 몽타주 재생
    UAbilityTask_PlayMontageAndWait* PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            TEXT("SheathKatana"),
            SheathMontage
        );

    PlayMontageTask->OnCompleted.AddDynamic(this, &UP4GA_SheathKatana::OnCompleteCallback);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &UP4GA_SheathKatana::OnInterruptedCallback);
    PlayMontageTask->ReadyForActivation();
}

void UP4GA_SheathKatana::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP4GA_SheathKatana::OnCompleteCallback()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UP4GA_SheathKatana::OnInterruptedCallback()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}