// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/P4MonsterBase.h"
#include "P4MonsterShamos.generated.h"

class AP4MonsterProjectile;
/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4MonsterShamos : public AP4MonsterBase
{
	GENERATED_BODY()

public:
	AP4MonsterShamos();

protected:
	virtual void AttackByAI() override;
	virtual void AttackHitCheck() override;

	virtual void SetupAttackDelegate() override;

	virtual void SetDead() override;
	
	FORCEINLINE virtual float GetAIAttackRange() override { return 1000.f; }

protected:
	void RangeAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollision;
	
	// DoubleWaterBomb 의 투사체 Class
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AP4MonsterProjectile> WaterProjectileClass;
};
