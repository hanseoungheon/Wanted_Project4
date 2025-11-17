// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_SheathKatana.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_SheathKatana : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UP4GA_SheathKatana();

    virtual bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags = nullptr,
        const FGameplayTagContainer* TargetTags = nullptr,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr
    ) const override;

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;

protected:
    UFUNCTION()
    void OnCompleteCallback();

    UFUNCTION()
    void OnInterruptedCallback();
};
