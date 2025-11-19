// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExec_MonsterGroggyGauge.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UGEExec_MonsterGroggyGauge : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UGEExec_MonsterGroggyGauge();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
