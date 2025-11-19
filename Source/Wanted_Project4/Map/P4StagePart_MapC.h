// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/P4StagePartBase.h"
#include "P4StagePart_MapC.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4StagePart_MapC : public AP4StagePartBase
{
	GENERATED_BODY()

public:
	AP4StagePart_MapC();
protected:
	virtual void BeginPlay() override;
	
};
