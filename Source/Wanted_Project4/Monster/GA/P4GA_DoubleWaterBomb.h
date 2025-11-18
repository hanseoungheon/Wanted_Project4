// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_DoubleWaterBomb.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_DoubleWaterBomb : public UGameplayAbility
{
	GENERATED_BODY()

	UP4GA_DoubleWaterBomb();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
