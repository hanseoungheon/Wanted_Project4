// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EnchantWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/P4InventoryWidget.h"
#include "UI/P4EnchantSlotWidget.h"
#include "Character/P4CharacterPlayer.h"
#include "Player/P4PlayerController.h"

void UP4EnchantWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AcceptButtton != nullptr)
	{
		AcceptButtton->OnClicked.RemoveDynamic(this, &UP4EnchantWidget::OnAcceptClicked);
		AcceptButtton->OnClicked.AddDynamic(this, &UP4EnchantWidget::OnAcceptClicked);
	}

	if (DeclineButton != nullptr)
	{
		DeclineButton->OnClicked.RemoveDynamic(this, &UP4EnchantWidget::OnDeclinClicked);
		DeclineButton->OnClicked.AddDynamic(this, &UP4EnchantWidget::OnDeclinClicked);
	}
}

void UP4EnchantWidget::SetEnchantNPC(TScriptInterface<IP4NPCEnchantWeaponInterface> InNPC)
{
	EnchantNPC = InNPC;
}

void UP4EnchantWidget::OnAcceptClicked()
{
	if (WeaponSlot == nullptr || StoneSlot == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("EnchantWidget: 슬롯 위젯이 세팅 안 됨"));
		return;
	}

	if (WeaponSlot->HasItem() == false || StoneSlot->HasItem() == false)
	{
		return;
	}

	const FInventoryItem& WeaponItem = WeaponSlot->GetStoredItem();
	const FInventoryItem& StoneItem = StoneSlot->GetStoredItem();

	StoneUpgradeType = StoneSlot->UpgradeType;
	
	if (EnchantNPC)
	{
		EnchantNPC->HandleEnchantWeapon();
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController<APlayerController>();
	AP4CharacterPlayer* Player = PC ? Cast<AP4CharacterPlayer>(PC->GetPawn()) : nullptr;

	if (Player == nullptr)
	{
		return;
	}

	if (UP4InventoryComponent* InvComp = Player->FindComponentByClass<UP4InventoryComponent>())
	{
		// 무기/강화석 하나씩 제거 (SlotIndex 사용)
		//InvComp->RemoveItem(WeaponItem.ItemData, 1, WeaponItem.SlotIndex);
		InvComp->RemoveItem(StoneItem.ItemData, 1, StoneItem.SlotIndex);
	}

	// 5) 슬롯 비우기
	WeaponSlot->ClearItem();
	StoneSlot->ClearItem();


	//입력 모드 되돌리기.
	AP4PlayerController* PlayerController
		= GetWorld()->GetFirstPlayerController<AP4PlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;
		PlayerController->Test();
		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}
	//수락 버튼 눌렀을 때 작동
	RemoveFromParent();
}

void UP4EnchantWidget::OnDeclinClicked()
{
	if (WeaponSlot != nullptr)
	{
		WeaponSlot->ClearItem();
	}

	if (StoneSlot != nullptr)
	{
		StoneSlot->ClearItem();
	}
	
	AP4PlayerController* PlayerController
		= GetWorld()->GetFirstPlayerController<AP4PlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;
		PlayerController->Test();
		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}
	//거절 버튼 눌렀을 때 작동
	RemoveFromParent();
}

EP4UpgradeType UP4EnchantWidget::GetUpgradeType() const
{
	return StoneUpgradeType;
}

//void UP4EnchantWidget::TryEnchant()
//{
//	if (WeaponSlot == nullptr || StoneSlot == nullptr)
//	{
//		UE_LOG(LogTemp, Error, TEXT("EnchantWidget: 슬롯 위젯이 세팅 안 됨"));
//		return;
//	}
//
//	if (WeaponSlot->HasItem() == false || StoneSlot->HasItem() == false)
//	{
//		return;
//	}
//
//	float Chance = CalculateChance(WeaponItem, StoneItem);
//
//	bool bSuccess = (FMath::FRand() <= Chance);
//
//	if (bSuccess)
//	{
//		WeaponItem->EnchantLevel++;
//		// 또는 GameplayEffect로 능력치 상승
//	}
//
//	// 강화석은 무조건 소모
//	Inventory->RemoveItem(StoneItem, 1);
//
//	RefreshUI();

//}
