// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P4MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4MonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AP4MonsterAIController();

	void RunAI();

	void StopAI();

	void SetPatternActive(bool InActive);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
protected:	
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
	
	
};
