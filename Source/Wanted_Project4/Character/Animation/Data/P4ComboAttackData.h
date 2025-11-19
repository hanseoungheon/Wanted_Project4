// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "P4ComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4ComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UP4ComboAttackData();

public:
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FRameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;

};
