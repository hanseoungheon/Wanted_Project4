// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Run.h"
#include "P4GA_Run.h"
#include "Character/P4CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UP4GA_Run::UP4GA_Run()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP4GA_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//속도 올리기
	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void UP4GA_Run::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//속도 원상복구
	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
