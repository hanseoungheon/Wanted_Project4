// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_Run.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_Run : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UP4GA_Run();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	const float RunSpeed = 1000.f;
	const float WalkSpeed = 300.f;

};
