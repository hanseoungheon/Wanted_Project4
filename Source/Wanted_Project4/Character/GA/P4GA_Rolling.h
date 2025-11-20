// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P4GA_Rolling.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GA_Rolling : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UP4GA_Rolling();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	//변수 섹션.
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Roll)
	TObjectPtr<class UAnimMontage>RollMontage;

protected:
	FName ChooseRollSection(const ACharacter* Character) const;

	UFUNCTION()
	void OnRollMontageEnded();
};
