// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4RespawnWidget.h"
#include "Components/Button.h"
#include "Player/P4PlayerController.h"

void UP4RespawnWidget::ShowRespawnUI()
{
	SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	//AP4CharacterBase* P4Character = Cast<AP4CharacterBase>(OwningActor);
	//P4Character->GetAbilitySystemComponent();
	
	AP4PlayerController* PC = Cast<AP4PlayerController>(OwningPawn->GetController());
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;

	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}

	// 버튼 바인딩
	if (RespawnButton)
	{
		RespawnButton->OnClicked.Clear();
		RespawnButton->OnClicked.AddDynamic(this, &UP4RespawnWidget::HandleRespawnButtonPressed);
	}	
}

void UP4RespawnWidget::HideRespawnUI()
{
	AP4PlayerController* PC = Cast<AP4PlayerController>(OwningPawn->GetController());
	// 게임 전용 모드
	PC->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	if (FadeOut)
	{
		PlayAnimation(FadeOut);
		// 애니메이션 끝난 후 Hidden 처리 가능
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UP4RespawnWidget::HandleRespawnButtonPressed()
{
	// PlayerController로 전달할 Delegate 호출
	OnRespawnClicked.Broadcast();
}
