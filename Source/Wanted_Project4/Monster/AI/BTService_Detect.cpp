// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MonsterAIInterface.h"
#include "Monster/P4BossMonsterBase.h"

UBTService_Detect::UBTService_Detect()
{
	// 에디터에서 보이는 서비스 이름 설정
	NodeName = TEXT("Detect");

	// TickNode 실행 간격
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 폰 접근
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		// 감지된 폰이 없으면 반환
		return;
	}

	// AI 인터페이스 형변환
	IMonsterAIInterface* AIPawn = Cast<IMonsterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		// 인터페이스가 없으면 반환
		return;
	}

	// 감지 범위(반지름)을 Pawn 한테 받기
	float DetectRange = AIPawn->GetAIDetectRange();

	// 감지 처리를 위한 월드 값
	UWorld* World = ControllingPawn->GetWorld();

	// 몬스터의 현재 위치를 시야 감지 위치 설정 (몬스터 위치 구하기)
	FVector MonsterPos = ControllingPawn->GetActorLocation();

	// 충돌 결과 배열 선언
	TArray<FOverlapResult> Overlaps;

	// 충돌 판정 시 본인을 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// 오버랩 함수를 통해 주변 Detect
	bool Result = World->OverlapMultiByChannel(
		Overlaps,
		MonsterPos,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRange),
		Params
	);

	// 충돌 감지 시
	if (Result)
	{
		// 충돌 결과 배열 순회
		for (const auto& OverlapResult : Overlaps)
		{
			// 충돌 감지 액터를 폰으로
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// 감지된 액터가 폰, 플레이어 컨트롤러이면
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 감지된 폰 정보를 블랙보드에 타겟으로
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Pawn);
				AIPawn->SetCharacterMovementSpeed(AIPawn->GetAIChaseSpeed());

				// 감지 영역 디버그
				DrawDebugSphere(
					World,
					MonsterPos,
					DetectRange,
					16,
					FColor::Red,
					false,
					0.5f
				);

				// 감지 지점 점으로 디버그
				DrawDebugPoint(
					World,
					Pawn->GetActorLocation(),
					10.f,
					FColor::Green,
					false,
					0.6f
				);

				// 감지 방향 선으로 디버그
				DrawDebugLine(
					World,
					MonsterPos,
					Pawn->GetActorLocation(),
					FColor::Green,
					false,
					0.5f
				);

				return;
			}
		}
	}

	// 감지 실패 시 타겟에 nullptr
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	AIPawn->SetCharacterMovementSpeed(AIPawn->GetAIMovementSpeed());

	// 감지 영역 빨간색으로 디버그
	DrawDebugSphere(
		World,
		MonsterPos,
		DetectRange,
		16,
		FColor::Red,
		false,
		0.5f
	);
}
