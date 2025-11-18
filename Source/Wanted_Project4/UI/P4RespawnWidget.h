// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "P4RespawnWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawnButtonClicked);

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4RespawnWidget : public UP4CustomWidget
{
	GENERATED_BODY()

public:
	/** UI 표시 함수 */
	UFUNCTION(BlueprintCallable)
	void ShowRespawnUI();

	/** UI 숨김 처리 */
	UFUNCTION(BlueprintCallable)
	void HideRespawnUI();

	/** Respawn 버튼에서 호출되는 함수 */
	UFUNCTION(BlueprintCallable)
	void HandleRespawnButtonPressed();

public:
	/** Respawn 버튼 눌리면 PlayerController에게 전달할 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Respawn")
	FOnRespawnButtonClicked OnRespawnClicked;

protected:
	// Respawn UI 애니메이션 (필요하면)
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeOut;

	// 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RespawnButton;
};
