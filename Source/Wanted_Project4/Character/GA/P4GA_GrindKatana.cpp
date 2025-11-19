// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_GrindKatana.h"
//#include "Character/P4CharacterBase.h"
#include "Character/P4CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Tag/P4GameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UP4GA_GrindKatana::UP4GA_GrindKatana()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_GrindKatana::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        UE_LOG(LogTemp, Error, TEXT("[GA_DrawKatana] Super::CanActivateAbility 실패 - 태그 차단됨"));
        return false;
    }

	return true;
}

void UP4GA_GrindKatana::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UE_LOG(LogTemp, Warning, TEXT("ActivateAbility"))
    AP4CharacterPlayer* Player = CastChecked<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());
    ASC = Player->GetAbilitySystemComponent();

    Player->GetCharacterMovement()->SetMovementMode(MOVE_None);

    // 공격당하면 실패 → 태그 감지
    UAbilityTask_WaitGameplayTagAdded* WaitDamaged = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, P4TAG_CHARACTER_ISDAMAGED);
    WaitDamaged->Added.AddDynamic(this, &UP4GA_GrindKatana::OnInterrupted);
    WaitDamaged->ReadyForActivation();

    // 몽타주 재생
    UAbilityTask_PlayMontageAndWait* PlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        TEXT("GrindKatana"),
        Player->GetGrindKatanaMontage(),
        1.0f
    );

    PlayTask->OnCompleted.AddDynamic(this, &UP4GA_GrindKatana::OnGrindComplete);
    PlayTask->ReadyForActivation();
}

void UP4GA_GrindKatana::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    OnInterrupted();
}

void UP4GA_GrindKatana::OnInterrupted()
{
    AP4CharacterBase* P4Character = Cast<AP4CharacterBase>(GetAvatarActorFromActorInfo());
    P4Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

// 성공적으로 완료될 때 실행
void UP4GA_GrindKatana::OnGrindComplete()
{
    AP4CharacterBase* P4Character = Cast<AP4CharacterBase>(GetAvatarActorFromActorInfo());
    P4Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    // todo: 보호막 부여
    if (ShieldEffectClass && ASC)
    {
        FGameplayEffectSpecHandle SpecHandle =
            ASC->MakeOutgoingSpec(ShieldEffectClass, 1.f, ASC->MakeEffectContext());

        if (SpecHandle.IsValid())
        {
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}