// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_Death.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_Death : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UP4GA_Death();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnJumpFinished();

	void HandleDeathNow(class AP4CharacterBase* Character);

private:
	//UPROPERTY()
	//TSubclassOf<class UP4RespawnWidget> RespawnWidgetClass;

	//TObjectPtr<UP4RespawnWidget> RespawnWidget;
	//DeathWidgetClass
};
