// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/P4MonsterPatternComponent.h"

#include "Monster/P4BossMonsterBase.h"
#include "Monster/P4MonsterBase.h"

// Sets default values for this component's properties
UP4MonsterPatternComponent::UP4MonsterPatternComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// 현재 패턴 풀을 입력받은 패턴 데이터 배열로 초기화
void UP4MonsterPatternComponent::InitializePatterns(const TArray<FPatternData>& InPatterns)
{
	PatternPool = InPatterns;
}

// 가중치 계산을 통해 패턴 선택, 실행함수 호출
void UP4MonsterPatternComponent::TryExecutePattern(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	// 몬스터 위치
	FVector MonsterLocation = GetOwner()->GetActorLocation();

	// 몬스터와 타겟의 거리
	float Distance = FVector::Dist(MonsterLocation, Target->GetActorLocation());

	// 후보 필터링
	// 사용 가능한 패턴 저장용 배열
	TArray<const FPatternData*> AvailablePatterns;
	for (const FPatternData& Pattern : PatternPool)
	{
		// IsPatternAvailable 함수 사용
		if (IsPatternAvailable(Pattern, Distance))
		{
			// 사용 가능 패턴을 배열에 저장
			AvailablePatterns.Add(&Pattern);
		}
	}

	// 사용 가능한 패턴이 없으면 반환
	if (AvailablePatterns.IsEmpty())
	{
		return;
	}

	// @Todo: 가중치 계산
	// 타겟과의 거리

	// 최근 사용 빈도

	// 음수 -> 0으로

	// 전체 가중치의 총합이 0
	// 사용 가능한 패턴 중 랜덤 1개 실행

	// 가중치를 이용해서 패턴 실행

	// @Todo: 임시로 균등 확률로 랜덤 사용
	int32 Index = FMath::RandRange(0, AvailablePatterns.Num() - 1);
	ExecutePattern(*AvailablePatterns[Index]);
}

bool UP4MonsterPatternComponent::IsPatternAvailable(const FPatternData& Pattern, float Distance) const
{
	// 사거리 필터링
	// 최소, 최대 사거리 밖이면 false 반환
	if (Distance < Pattern.MinDistance || Distance > Pattern.MaxDistance)
	{
		return false;
	}

	// 쿨타임 필터링
	// 패턴별 타이머 받아오기
	const float* LastTime = PatternCooldownTimers.Find(Pattern.PatternName);

	// 현재 타임 받기
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// @MobTODO: 패턴 쿨타임 확인
	if (LastTime)
	{
		UE_LOG(LogTemp, Log, TEXT("[MobSkillCoolTime] %s CoolTime : %f"), *Pattern.PatternName.ToString(), (CurrentTime - *LastTime));
	}

	// 패턴 별 타이머가 존재하고 쿨타임보다 덜 지났으면 false 반환
	if (LastTime && ((CurrentTime - *LastTime) < Pattern.Cooldown))
	{
		return false;
	}

	return true;
}

void UP4MonsterPatternComponent::ExecutePattern(const FPatternData& Pattern)
{
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(GetOwner());
	if (Monster)
	{
		UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
		if (ASC)
		{
			// AbilityTag 를 통해 패턴 Ability 찾기
			FString PatternName = TEXT("Monster.Action.");
			PatternName.Append(Pattern.PatternName.ToString());

			FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(*PatternName);

			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));

			// 사용한 패턴 쿨타임 기록
			PatternCooldownTimers.Add(Pattern.PatternName, GetWorld()->GetTimeSeconds());
		}
	}
}
