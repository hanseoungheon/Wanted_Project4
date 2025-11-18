// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/P4UpgradeType.h"
#include "P4UpgrageWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP4UpgrageWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WANTED_PROJECT4_API IP4UpgrageWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EP4UpgradeType GetUpgradeType() const = 0;

};
