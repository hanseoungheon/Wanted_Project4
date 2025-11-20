// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/BTService_CheckAttackRange.h"

#include "AIController.h"
#include "P4MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	APawn* AIPawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (!AIPawn|| !TargetActor)
	{
		return;
	}

	float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
	IMonsterAIInterface* Monster = Cast<IMonsterAIInterface>(AIController->GetPawn());
	if (Monster)
	{
		float AttackRange = Monster->GetAIAttackRange();
		if (Distance <= AttackRange)
		{
			AIController->StopMovement();
		}
	}
}
