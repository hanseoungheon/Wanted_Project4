// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4QuestInformationWidget.h"
#include "Game/P4GameInstance.h"
#include "NPC/Quest/P4QuestManager.h"
#include "Components/TextBlock.h"

void UP4QuestInformationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		UP4GameInstance* GI = world->GetGameInstance<UP4GameInstance>(); //싱글톤 게임 인스턴스 받아오기.

		if (GI != nullptr)
		{
			QuestManager = GI->QuestManager;
		}
	}

	RefreshQuestUI();

	if (QuestManager != nullptr)
	{
		//퀘스트가 시작되면 보이게 하는 함수를 델리게이트로 연결.
		QuestManager->OnQuestStarted.AddUObject(this, &UP4QuestInformationWidget::HandleQuestStarted);
		//퀘스트 진행사항이 업데이트하면 갱신되게 하는 함수를 델리게이트로 연결.
		QuestManager->OnQuestUpdated.AddUObject(this, &UP4QuestInformationWidget::HandleQuestUpdated);
		//퀘스트가 종료되면 UI를 숨기게 하는 함수를 델리게이트로 연결.
		QuestManager->OnQuestCleared.AddDynamic(this, &UP4QuestInformationWidget::HandleQuestCleared);
	}

	//시작할땐 숨기기.
	SetVisibility(ESlateVisibility::Hidden);
}

void UP4QuestInformationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//RefreshQuestUI();
}

void UP4QuestInformationWidget::RefreshQuestUI()
{
	if (QuestName == nullptr || Stage == nullptr ||
		CurrentProgress == nullptr || ObjectiveProgress == nullptr)
	{
		return;
	}

	//현재 퀘스트하고 현재 퀘스트의 스테이지 가져오기.
	const FP4QuestInfo* Quest = QuestManager->GetCurrentQuest();
	const FP4StageDetails* StageInfo = QuestManager->GetCurrentStage();

	if (Quest == nullptr || StageInfo == nullptr)
	{
		return;
	}

	QuestName->SetText(Quest->QuestName);
	Stage->SetText(StageInfo->StageName);

	if (StageInfo->Objectives.Num() > 0)
	{
		const FP4ObjectiveDetails& Obj = StageInfo->Objectives[0];

		int32 Cur = QuestManager->GetObjectiveProgress(Obj.ObjectiveID);
		int32 Target = Obj.Quantity;

		CurrentProgress->SetText(FText::AsNumber(Cur));
		ObjectiveProgress->SetText(FText::AsNumber(Target));
	}

	else
	{
		CurrentProgress->SetText(FText::FromString(TEXT("0")));
		ObjectiveProgress->SetText(FText::FromString(TEXT("0")));
	}
}

void UP4QuestInformationWidget::HandleQuestStarted()
{
	// 퀘스트 시작되면 보이게
	SetVisibility(ESlateVisibility::Visible);

	RefreshQuestUI();
}

void UP4QuestInformationWidget::HandleQuestUpdated()
{
	RefreshQuestUI();
}

void UP4QuestInformationWidget::HandleQuestCleared(int32 ClearedQuestCode)
{
	// 퀘스트 끝나면 숨기기
	SetVisibility(ESlateVisibility::Hidden);
}
