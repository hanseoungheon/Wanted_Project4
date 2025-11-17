// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Jump.h"
#include "GameFramework/Character.h"
#include "AT/P4AT_JumpAndWaitForLanding.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

UP4GA_Jump::UP4GA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}


	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UP4GA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UP4AT_JumpAndWaitForLanding* JumpAndWaitForLandingTask = UP4AT_JumpAndWaitForLanding::CreateTask(this);
	JumpAndWaitForLandingTask->OnComplete.AddDynamic(this, &UP4GA_Jump::OnCompletedCallback);
	JumpAndWaitForLandingTask->ReadyForActivation();
}

void UP4GA_Jump::InputReleased (const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UP4GA_Jump::OnCompletedCallback()	//OnLanded
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
