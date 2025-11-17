// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/P4MonsterAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"

AP4MonsterAIController::AP4MonsterAIController()
{
	
}

void AP4MonsterAIController::RunAI()
{	
	// 블랙보드 컴포넌트 받기
	UBlackboardComponent* BBComp = Blackboard.Get();
	
	// 블랙보드 사용 설정
	if (UseBlackboard(BBAsset, BBComp))
	{
		// 블랙보드 키 값 초기화
		FVector HomePos = GetPawn()->GetActorLocation();
		Blackboard->SetValueAsVector(TEXT("HomePos"), HomePos);

		// BT 실행
		bool Result = RunBehaviorTree(BTAsset);

		// 어썰트
		ensureAlways(Result);
	}
}

void AP4MonsterAIController::StopAI()
{
	// 실행중인 BT 컴포넌트 받기
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void AP4MonsterAIController::SetPatternActive(bool InActive)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	// 블랙보드의 IsPatternActive 세팅
	BB->SetValueAsBool("IsPatternActive", InActive);

	// 만약 패턴이 발동 중이면 이동을 중단
	if (InActive)
	{
		StopMovement();
	}
}

void AP4MonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	IMonsterAIInterface* Monster = Cast<IMonsterAIInterface>(InPawn);
	if (Monster)
	{
		BBAsset = Monster->GetBBAsset();
		BTAsset = Monster->GetBTAsset();
	}
	
	// 컨트롤러가 Pawn 에 빙의하면 AI 실행
	RunAI();
}
