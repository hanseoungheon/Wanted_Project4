// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P4HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4HUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UP4HUDWidget(const FObjectInitializer& ObjectInitalizer);

	void UpdateHpBar(float NewCurrentHp);

	void UpdataMiniMap(FVector PlayerLocation);

	void SetBoss(AActor* NewBoss);
	TObjectPtr<class UP4HpBarWidget> GetHpBar() const;

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UP4HpBarWidget> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UP4BossHpBar> WidgetBossHpBar;

	//UPROPERTY()
	//TObjectPtr<class UP4MiniMapWidget> MiniMap;
	TObjectPtr<class UP4QuestInformationWidget> QuestTextBlock;

};
