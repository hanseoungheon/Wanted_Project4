// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/P4GameInstance.h"
#include "NPC/Quest/P4QuestManager.h"
#include "Kismet/GameplayStatics.h"

UP4GameInstance::UP4GameInstance()
{
}

void UP4GameInstance::Init()
{
	Super::Init();

	QuestManager = NewObject<UP4QuestManager>(this);
	//QuestManager->AddToRoot(); //가비지 컬렉터가 납치 못하게 방지인데 이거 함부로 쓰면 안댈듯 겜 크래시나요

	//QuestManager->StartQuest(1);	

	if (QuestManager != nullptr)
	{
		QuestManager->OnQuestCleared.AddDynamic(this, &UP4GameInstance::HandleClearQuest);
	}
}

void UP4GameInstance::HandleClearQuest(int32 ClearQuestCode)
{
	if (ClearQuestCode == 1)
	{
		FName NextLevelName(TEXT("BattleAeranMap"));

		UGameplayStatics::OpenLevel(this, NextLevelName);
	}


}
