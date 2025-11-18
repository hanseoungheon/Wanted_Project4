// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Death.h"
#include "Character/P4CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "UI/P4RespawnWidget.h"
#include "Player/P4PlayerController.h"

UP4GA_Death::UP4GA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP4GA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AP4CharacterBase* Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC)
        return;

    if (ASC->HasMatchingGameplayTag(P4TAG_CHARACTER_ISJUMPING))
    {
        //UE_LOG(LogTemp, Warning, TEXT("Death: Jumping active! Waiting finish..."));

        UAbilityTask_WaitGameplayTagRemoved* WaitTask =
            UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(
                this,
                P4TAG_CHARACTER_ISJUMPING
            );

        WaitTask->Removed.AddDynamic(this, &UP4GA_Death::OnJumpFinished);
        WaitTask->ReadyForActivation();

        return; // 점프 끝날 때까지 Death 처리 보류
    }

    HandleDeathNow(Character);
}

void UP4GA_Death::OnJumpFinished()
{
    AP4CharacterBase* Character = Cast<AP4CharacterBase>(GetAvatarActorFromActorInfo());

    if (Character)
    {
        HandleDeathNow(Character);
    }
}

void UP4GA_Death::HandleDeathNow(AP4CharacterBase* Character)
{
    //UE_LOG(LogTemp, Warning, TEXT("Death process started."));

    //Character->DisableInput(nullptr);
    // 이동 차단
    Character->GetCharacterMovement()->SetMovementMode(MOVE_None);

    // 충돌 비활성
    Character->SetActorEnableCollision(false);

    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    // 모든 Tag 해제
    //ASC->RemoveLooseGameplayTags(ASC->GetOwnedGameplayTags());
    // 죽음 태그 부착
    //ASC->AddLooseGameplayTag(P4TAG_CHARACTER_ISDEAD);

    // 죽음 시퀀스 실행, UI 표시 등
    if (AP4PlayerController* PC = Cast<AP4PlayerController>(Character->GetController()))
    {
        if (PC->RespawnWidget)
        {
            PC->RespawnWidget->ShowRespawnUI();
        }
    }

    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}