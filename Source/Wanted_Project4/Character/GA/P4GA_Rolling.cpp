// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Rolling.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Interface/P4RollingInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"

UP4GA_Rolling::UP4GA_Rolling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontageRef(TEXT("/Game/Character/Animation/AM_Rolling.AM_Rolling"));

	if (RollMontageRef.Succeeded() == true)
	{
		RollMontage = RollMontageRef.Object;
	}
}

void UP4GA_Rolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());	

	if (Character == nullptr || RollMontage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector MoveDir = Character->GetLastMovementInputVector();
	if (MoveDir.IsNearlyZero())
	{
		MoveDir = Character->GetActorForwardVector();
	}
	
	MoveDir.Normalize();

	IP4RollingInterface* RollInterface = Cast<IP4RollingInterface>(Character);

	if (RollInterface != nullptr)
	{
		RollInterface->SetRollingDirection(MoveDir);
	}
	
	const FName SectionName = ChooseRollSection(Character);

	auto* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		RollMontage,
		1.0f,
		SectionName
	);
	Task->OnCompleted.AddDynamic(this, &UP4GA_Rolling::OnRollMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UP4GA_Rolling::OnRollMontageEnded);
	Task->OnCancelled.AddDynamic(this, &UP4GA_Rolling::OnRollMontageEnded);

	Task->ReadyForActivation();
}

void UP4GA_Rolling::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UP4GA_Rolling::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_Rolling::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UP4GA_Rolling::ChooseRollSection(const ACharacter* Character) const
{
	FVector MoveInput = Character->GetLastMovementInputVector();

	if (MoveInput.IsNearlyZero())
	{
		return FName("Rolling_F");
	}

	MoveInput.Normalize();

	const FVector Forward = Character->GetActorForwardVector();
	const FVector Right = Character->GetActorRightVector();

	float ForwardDot = FVector::DotProduct(MoveInput, Forward);
	float RightDot = FVector::DotProduct(MoveInput, Right);

	//╬у ╣з.
	if (ForwardDot > 0.7)
	{
		return FName("Rolling_F");
	}
	else if (ForwardDot < -0.7)
	{
		return FName("Rolling_B");
	}

	//аб ©Л.
	if (RightDot > 0.0f)
	{
		return FName("Rolling_R");
	}
	else
	{
		return FName("Rolling_L");
	}

	//return FName("Rollong_F");
}

void UP4GA_Rolling::OnRollMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}