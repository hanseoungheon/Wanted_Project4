// Fill out your copyright notice in the Description page of Project Settings.


#include "P4QuestWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Game/P4GameInstance.h"
#include "NPC/Quest/P4QuestManager.h"

void UP4QuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* world = GetWorld();

	if (world != nullptr)
	{
		UP4GameInstance* GI = world->GetGameInstance<UP4GameInstance>();

		if (GI != nullptr)
		{
			QuestManager = GI->QuestManager;
		}
	}

	//버튼 델리게이트 생성.
	if (AcceptButtton != nullptr)
	{
		AcceptButtton->OnClicked.RemoveDynamic(this, &UP4QuestWidget::OnAcceptClicked);
		AcceptButtton->OnClicked.AddDynamic(this, &UP4QuestWidget::OnAcceptClicked);
	}

	if (DeclineButton != nullptr)
	{
		DeclineButton->OnClicked.RemoveDynamic(this, &UP4QuestWidget::OnDeclinClicked);
		DeclineButton->OnClicked.AddDynamic(this, &UP4QuestWidget::OnDeclinClicked);
	}
}

void UP4QuestWidget::InitWithQuestCode(int32 InQuestCode)
{
	PendingQuestCode = InQuestCode;

	if (QuestManager == nullptr)
	{
		return;
	}

	const FP4QuestInfo* Quest = QuestManager->GetQuest(PendingQuestCode);

	if (Quest == nullptr)
	{
		return;
	}

	if (QuestNameText != nullptr)
	{
		QuestNameText->SetText(Quest->QuestName);
	}

	if (QuestDescText != nullptr)
	{
		FString DescString = Quest->LogDescription.ToString();

		DescString.ReplaceInline(TEXT("\\n"), TEXT("\n"));

		QuestDescText->SetText(FText::FromString(DescString));
	}

	if (StageInfoText != nullptr)
	{
		if (Quest->Stages.Num() > 0)
		{
			const FP4StageDetails& FirstStage = Quest->Stages[0];

			FString StageStr = FString::Printf(
				TEXT("%s"), 
				*FirstStage.Description.ToString()
			);

			StageInfoText->SetText(FText::FromString(StageStr));
		}
		else
		{
			StageInfoText->SetText(FText::GetEmpty());
		}
	}
}
void UP4QuestWidget::OnAcceptClicked()
{
	//퀘스트 실행하기.
	if (QuestManager != nullptr && PendingQuestCode != -1)
	{
		QuestManager->StartQuest(PendingQuestCode);
	}

	APlayerController* PlayerController 
		= GetWorld()->GetFirstPlayerController<APlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;

		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}
	//닫기.
	RemoveFromParent();
}

void UP4QuestWidget::OnDeclinClicked()
{
	APlayerController* PlayerController
		= GetWorld()->GetFirstPlayerController<APlayerController>();

	if (PlayerController != nullptr)
	{
		FInputModeGameOnly GameOnly;

		PlayerController->SetInputMode(GameOnly);
		PlayerController->bShowMouseCursor = false;
	}

	RemoveFromParent();
}
