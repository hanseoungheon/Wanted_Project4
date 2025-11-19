// Fill out your copyright notice in the Description page of Project Settings.
/**
* 작성 - 한승헌
* 일시 = 2025.11.13
*
*
*
**/
#pragma once

#include "CoreMinimal.h"
#include "P4QuestInfo.h"
#include "P4ObjectiveDetails.h"
#include "P4StageDetails.h"
#include "UObject/NoExportTypes.h"
#include "P4QuestManager.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnQuestStarted);
DECLARE_MULTICAST_DELEGATE(FOnQuestUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCleared, int32, ClearedQuestCode);

UCLASS()
class WANTED_PROJECT4_API UP4QuestManager : public UObject
{
	GENERATED_BODY()
	
	//함수 섹션
public:
	UP4QuestManager();

	const FP4QuestInfo* GetQuest(int32 InQuestCode) const; //퀘스트 ID를 통해서 퀘스트 가져오기.

	void StartQuest(int32 QuestCode); //퀘스트 시작(진행 사항 초기화)

	void UpdateObjective(const FString& ObjectiveID); //ObjectiveID에 해당하는 목표 진행도 갱신,설정.

	UFUNCTION(BlueprintPure, Category = Quest)
	int32 GetObjectiveProgress(const FString& ObjectiveID) const; //현재 Object(퀘스트목표) 진행도 조회.

	UFUNCTION(BlueprintPure, Category = Quest)

	FORCEINLINE bool IsQuestCleared(int32 QuestCode) const
	{
		return ClearedQuestCodes.Contains(QuestCode);
	}

	const FP4QuestInfo* GetCurrentQuest() const;
	const FP4StageDetails* GetCurrentStage() const;

	//Getter 섹션.
public:
	UFUNCTION(BlueprintPure, Category = Quest)
	FORCEINLINE bool IsQuestActive() const //현재 퀘스트가 진행중인지 확인하는 함수.
	{
		return bQuestActive;
	}

	UFUNCTION(BlueprintPure, Category = Quest)
	FORCEINLINE int32 GetCurrentStageIndex() const 
	{ 
		return CurrentStageIndex; 
	}

	UFUNCTION(BlueprintPure, Category = Quest)
	FORCEINLINE int32 GetCurrentQuestIndex() const
	{
		return CurrentQuestCode;
	}
protected:

	//변수 섹션
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	UDataTable* QuestTable; //퀘스트 데이터 테이블

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	int32 CurrentQuestCode; //찾으려는 퀘스트의 코드.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	int32 CurrentStageIndex; //현재 스테이지 인덱스.

	TMap<FString, int32> ObjectiveProgress; //ObjectiveId를 키로 사용해서 현재 카운트에 접근.

	//델리게이트
	FOnQuestStarted OnQuestStarted;
	FOnQuestUpdated OnQuestUpdated;
	FOnQuestCleared OnQuestCleared;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	bool bQuestActive = false;

	TSet<int32> ClearedQuestCodes;
};
