// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EnchantWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

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
	if (EnchantNPC)
	{
		EnchantNPC->HandleEnchantWeapon();
	}

	//입력 모드 되돌리기.
	APlayerController* PlayerController
		= GetWorld()->GetFirstPlayerController<APlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;

		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}
	//수락 버튼 눌렀을 때 작동
	RemoveFromParent();
}

void UP4EnchantWidget::OnDeclinClicked()
{
	APlayerController* PlayerController
		= GetWorld()->GetFirstPlayerController<APlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;

		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}
	//거절 버튼 눌렀을 때 작동
	RemoveFromParent();
}
