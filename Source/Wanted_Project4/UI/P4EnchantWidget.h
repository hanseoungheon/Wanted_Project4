// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "Interface/P4NPCEnchantWeaponInterface.h"
#include "P4EnchantWidget.generated.h"

/**
 * 
 */


UCLASS()
class WANTED_PROJECT4_API UP4EnchantWidget : public UP4CustomWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// NPC 인터페이스를 밖에서 세팅해주는 함수
	void SetEnchantNPC(TScriptInterface<IP4NPCEnchantWeaponInterface> InNPC);

protected:
	UFUNCTION(BlueprintCallable, Category = Enchant)
	void OnAcceptClicked(); //수락버튼 클릭할때 발생하는 코드.


	UFUNCTION(BlueprintCallable, Category = Enchant)
	void OnDeclinClicked(); //거절버튼 누를때 발생하는 코드.

	//UFUNCTION(BlueprintCallable, Category = Enchant)

	//void TryEnchant();
		
public:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> AcceptButtton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> DeclineButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UP4EnchantSlotWidget> WeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UP4EnchantSlotWidget> StoneSlot;

	//// 결과 텍스트 (성공/실패 표시용, BP에서 만들어도 되고 안 써도 됨)
	//UPROPERTY(meta = (BindWidgetOptional))
	//TObjectPtr<UTextBlock> ResultText;

	UPROPERTY()
	TObjectPtr<class UP4InventoryComponent> InventoryComp;

	UPROPERTY()
	TScriptInterface<IP4NPCEnchantWeaponInterface> EnchantNPC;


};
