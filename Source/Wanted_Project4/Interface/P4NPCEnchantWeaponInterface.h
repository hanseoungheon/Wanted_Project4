// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P4NPCEnchantWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP4NPCEnchantWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 작성 - 한승헌
 * 일시 - 2025.11.17
 * 내용 - 무기강화를 위한 인터페이스 함수입니다.
 */
class WANTED_PROJECT4_API IP4NPCEnchantWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//무기 강화시키는 함수.
	virtual void HandleEnchantWeapon() = 0;

};
