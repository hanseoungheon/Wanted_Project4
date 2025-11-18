// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P4GameMode.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP4GameMode();

protected:
	virtual void BeginPlay() override;
	
};
