// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P4RollingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP4RollingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WANTED_PROJECT4_API IP4RollingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetRollingDirection(const FVector InRollingDirection) = 0;
	virtual FVector GetRollingDirection() const = 0;

};
