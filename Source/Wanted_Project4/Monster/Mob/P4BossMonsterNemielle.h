// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P4MonsterProjectile.h"
#include "Monster/P4BossMonsterBase.h"
#include "P4BossMonsterNemielle.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4BossMonsterNemielle : public AP4BossMonsterBase
{
	GENERATED_BODY()
	
public:
	AP4BossMonsterNemielle();

	virtual void BeginPlay() override;
	
	virtual void SetupAttackDelegate() override;

protected:
	// DoubleWaterBomb 의 투사체 Class
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AP4MonsterProjectile> WaterProjectileClass;
	
protected:
	void LeftWingStomp();
	void Howling();
	void EnergyBomb();
	void DashAttack();
	void DoubleWaterBomb();
};
