// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
protected:
	void LeftWingStomp();
	void Howling();
	void EnergyBomb();
	void DashAttack();
};
