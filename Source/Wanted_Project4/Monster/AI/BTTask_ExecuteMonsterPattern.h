// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteMonsterPattern.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UBTTask_ExecuteMonsterPattern : public UBTTaskNode
{
	GENERATED_BODY()

	
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
