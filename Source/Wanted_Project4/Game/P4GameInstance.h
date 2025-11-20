// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Quest/P4QuestManager.h"
#include "Engine/GameInstance.h"
#include "P4GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4GameInstance : public UGameInstance
{
	GENERATED_BODY()
	//함수 섹션.
public:
	UP4GameInstance();

	virtual void Init() override;
protected:

	//변수섹션.
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	TObjectPtr<class UP4QuestManager> QuestManager; //퀘스트 매니저 -> BP에서 None으로 보일텐데 Init때문에 알아서 실행되니깐 None으로 보이는게 정상입니다.

protected:
	UFUNCTION()
	void HandleClearQuest(int32 ClearQuestCode);

	UFUNCTION()
	void ChangingLevel(FName NewLevelName);
};
