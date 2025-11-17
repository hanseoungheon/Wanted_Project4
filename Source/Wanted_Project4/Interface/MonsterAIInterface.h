// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "UObject/Interface.h"
#include "MonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

// 공격 종료 델리게이트 선언
DECLARE_DELEGATE(FAIMonsterAttackFinished);

/**
 * 
 */
class WANTED_PROJECT4_API IMonsterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	// AI 관련해서 구현 필요한 함수
	// BB, BT 불러오기
	virtual UBlackboardData* GetBBAsset() const = 0;
	virtual UBehaviorTree* GetBTAsset() const = 0;
	
	// AttributeSet 에 있음
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIChaseRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual float GetAIChaseSpeed() = 0;
	virtual float GetAIMovementSpeed() = 0;

	// @Todo: AttributeSet 에 없는 애들을 일단 어떻게 할 것인가
	// AttributeSet 에 없음
	virtual float GetAIAttackRange() = 0;
	virtual float GetAIPatrolRadius() = 0;

	// 공격, 피격 재생 여부
	virtual bool GetIsDamaged() = 0;
	virtual bool GetIsAttacking() = 0;
	
	// 공격 요청 함수
	virtual void AttackByAI() = 0;

	// 이동 속도 조정 함수
	virtual void SetCharacterMovementSpeed(float InSpeed) = 0;

	virtual void ExecuteAttackSection(const FName& SectionName) = 0;

	// 공격 종료 시점 델리게이트 호출 함수 (종료 시점임을 알림)
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) = 0;
};
