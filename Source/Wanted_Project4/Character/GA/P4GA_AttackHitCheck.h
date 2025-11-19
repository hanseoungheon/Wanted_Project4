// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UP4GA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	//UPROPERTY(EditAnywhere, Category = "GAS")
	//TSubclassOf<class UGameplayEffect> AttackDamageEffect;

// -작성: 노현기 -일시: 2025.11.19
protected:
	const FGameplayEventData* CurrentEventData = nullptr;
};
