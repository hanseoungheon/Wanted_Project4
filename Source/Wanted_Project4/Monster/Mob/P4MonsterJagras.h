// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../P4MonsterBase.h"
#include "P4MonsterJagras.generated.h"
/**
 * 
 */

UCLASS()
class WANTED_PROJECT4_API AP4MonsterJagras : public AP4MonsterBase
{
	GENERATED_BODY()

public:
	AP4MonsterJagras();

protected:
	virtual void AttackByAI() override;
	virtual void AttackHitCheck() override;

	virtual void SetupAttackDelegate() override;

	virtual float GetAIAttackRange() override { return 150.f; }

	// 몬스터 스킬별 공격 판정 함수


	//작성 - 한승헌
	//일시 - 2025.11.14
	//자그라스 죽으면 메세지 전송하는거를 위한 SetDead 오버라이딩.
	virtual void SetDead() override;

protected:
	void MeleeAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollision;
};
