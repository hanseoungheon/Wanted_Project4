// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/P4NPCBase.h"
#include "Interface/P4NPCEnchantWeaponInterface.h"
#include "P4CatWoman.generated.h"

/**
 * 작성- 한승헌
 * 일시- 2025.11.17
 * 내용- 강화시켜주는 NPC코드.
 */
UCLASS()
class WANTED_PROJECT4_API AP4CatWoman : public AP4NPCBase, public IP4NPCEnchantWeaponInterface
{
	GENERATED_BODY()

public:
	AP4CatWoman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void ShowEnchantUI() override;

	virtual void HandleEnchantWeapon() override;

protected:
	//GAS 섹션
public:
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag TAG_Data_Weapon_AttackRate;

	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag TAG_Data_Weapon_MaxHealths;
	
};
