// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P4MonsterPatternComponent.generated.h"

/*
 * 클래스 명 : P4MonsterPatternComponent
 * 역할 : 몬스터 패턴들에 대해서 쿨타임 관리 및 패턴 선택 및 실행 관리하는 컴포넌트
 */

// 패턴들의 데이터를 구조체로 생성
USTRUCT(Blueprintable)
struct FPatternData : public FTableRowBase
{
	GENERATED_BODY()

	FPatternData() {}
	FPatternData(
		FName InPatternName,
		float InCooldown,
		float InMinDistance,
		float InMaxDistance,
		float InAttackRange,
		float InAttackRadius,
		float InWeight)
		: PatternName(InPatternName), MinDistance(InMinDistance), MaxDistance(InMaxDistance),
		  AttackRange(InAttackRange), AttackRadius(InAttackRadius), Weight(InWeight)
	{
	}

	// 패턴 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PatternName;

	// 기술 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 5.f;

	// 패턴 발동 최소 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance = 0.f;

	// 패턴 발동 최대 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 1000.f;

	// 패턴 시전 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 0.f;

	// 패턴 시전 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius = 0.f;

	// 패턴 선택 확률 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WANTED_PROJECT4_API UP4MonsterPatternComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UP4MonsterPatternComponent();

public:
	// 패턴 풀 초기화
	void InitializePatterns(const TArray<FPatternData>& InPatterns);

	// 패턴 선택 및 실행
	void TryExecutePattern(AActor* Target);

private:
	// 패턴 목록 (패턴 데이터 배열)
	TArray<FPatternData> PatternPool;

	// 각 패턴별 쿨타임 타이머
	TMap<FName, float> PatternCooldownTimers;

	// 패턴 사용 가능 여부 반환 함수
	bool IsPatternAvailable(const FPatternData& Pattern, float Distance) const;

	// 해당 패턴 실행 함수
	void ExecutePattern(const FPatternData& Pattern);
};
