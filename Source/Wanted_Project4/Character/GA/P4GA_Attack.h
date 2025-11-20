// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UP4GA_Attack();

public:
	// -작성: 노현기 -일시: 2025.11.19
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();
	//void StartComboTimer();
	//void CheckComboInput();

	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> CurrentMontageTask;

protected:
	uint8 CurrentIndex = 0;
	uint8 MaxCount = 2;

	//UPROPERTY()
	//TObjectPtr<class UABComboActionData> CurrentComboData;

	//uint8 CurrentCombo = 0;
	//FTimerHandle ComboTimerHandle;
	//bool HasNextComboInput = false;
};
