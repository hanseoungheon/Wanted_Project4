// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "P4QuestInformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4QuestInformationWidget : public UP4CustomWidget
{
	GENERATED_BODY()
	//함수 섹션.
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void RefreshQuestUI();


	//델리게이트용 함수.
	UFUNCTION()
	void HandleQuestStarted();
	UFUNCTION()
	void HandleQuestUpdated();
	UFUNCTION()
	void HandleQuestCleared(int32 ClearedQuestCode);

	//변수 섹션
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuestName; // 현재 퀘스트의 이름.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Stage; // 현재 퀘스트 스테이지.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentProgress; // 현재 퀘스트의 진척도

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ObjectiveProgress; // 현재 퀘스트의 목표치

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	TObjectPtr<class UP4QuestManager> QuestManager;

protected:

	
};
