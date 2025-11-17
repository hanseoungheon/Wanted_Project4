// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/BTTask_ExecuteMonsterPattern.h"

EBTNodeResult::Type UBTTask_ExecuteMonsterPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 패턴 실행은 C++ 와 GAS 로 하기 때문에 아무것도 안해도 됨
	return EBTNodeResult::Succeeded;
}
