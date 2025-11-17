// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Attack.h"
#include "Character/P4CharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
//#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P4GA_Jump.h"
//#include "Character/ABComboActionData.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

UP4GA_Attack::UP4GA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP4GA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	//CurrentComboData = P4Character->GetComboActionData();
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);



	//FVector ForwardDir = P4Character->GetActorForwardVector();
	//float RollDistance = 9000.f; // 구르기 거리
	//float RollDuration = 1.f;  // 구르기 시간

	//// 이동 속도 계산
	//FVector LaunchVelocity = ForwardDir * (RollDistance / RollDuration);

	//// 움직임 중 중력 무시
	//P4Character->LaunchCharacter(LaunchVelocity, true, false);

	UAnimMontage* DefaultAttackMontage = P4Character->GetDefaultAttackMontage();
	if (DefaultAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), DefaultAttackMontage,1.f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP4GA_Attack::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP4GA_Attack::OnInterruptedCallback);

		// AT의 함수 호출
		PlayAttackTask->ReadyForActivation();
	}
		

	//StartComboTimer();
}

FName UP4GA_Attack::GetNextSection()
{
	CurrentIndex = (CurrentIndex % MaxCount) + 1;

	//FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentIndex);
	FName NextSection = *FString::Printf(TEXT("DefaultAttack%d"), CurrentIndex);
	return NextSection;
}

void UP4GA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//UE_LOG(LogTemp, Log, TEXT("InputPressed"));

	//if (!ComboTimerHandle.IsValid())
	//{
	//	HasNextComboInput = false;
	//}
	//else
	//{
	//	HasNextComboInput = true;
	//}
}

void UP4GA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//CurrentComboData = nullptr;
	//CurrentCombo = 0;
	//HasNextComboInput = false;
}

void UP4GA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UP4GA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
