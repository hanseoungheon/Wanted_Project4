// Fill out your copyright notice in the Description page of Project Settings.


#include "P4QuestManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

UP4QuestManager::UP4QuestManager()
{
	CurrentQuestCode = -1;
	CurrentStageIndex = 0;

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestTableRef(TEXT("/Game/Quest/QuestData.QuestData"));

	if (QuestTableRef.Succeeded())
	{
		QuestTable = QuestTableRef.Object;
	}
}

const FP4QuestInfo* UP4QuestManager::GetQuest(int32 InQuestCode) const
{
	ensureAlways(QuestTable != nullptr);

	static const FString Context(TEXT("QuestLookUp"));

	const TArray<FName> RowNames = QuestTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		const FP4QuestInfo* Row = QuestTable->FindRow<FP4QuestInfo>(RowName, Context);

		if (Row != nullptr && Row->QuestCode == InQuestCode) //만약 퀘스트의 식별번호랑 인자의 정수형이 같다면 출력.
		{
			return Row;
		}
	}
	return nullptr;
}


void UP4QuestManager::StartQuest(int32 QuestCode)
{
	const FP4QuestInfo* Quest = GetQuest(QuestCode);

	if (Quest == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("비상! 퀘스트가 NullPtr임!"));
		return;
	}

	CurrentQuestCode = QuestCode;
	CurrentStageIndex = 0;
	bQuestActive = true;
	ObjectiveProgress.Empty();


	if (Quest->Stages.IsValidIndex(CurrentStageIndex) == true) //Stage는 QuestInfo에서 FP4StageDetail의 구조체를 TArray로 감싸서 사용중.
	{
		const FP4StageDetails& FirstStage = Quest->Stages[CurrentStageIndex]; //해당 FP4StageDetails 가져오기.

		for (const FP4ObjectiveDetails& Obj : FirstStage.Objectives)
		{
			if (Obj.ObjectiveID.IsEmpty() == false)
			{
				ObjectiveProgress.Add(Obj.ObjectiveID, 0);
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Quest %d Started: %s"),
		QuestCode,
		*Quest->QuestName.ToString());

	OnQuestStarted.Broadcast();
}

void UP4QuestManager::UpdateObjective(const FString& ObjectiveID)
{
	if (bQuestActive == false || CurrentQuestCode == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 퀘스트가 없습니다."));
		return;
	}

	const FP4QuestInfo* Quest = GetQuest(CurrentQuestCode);

	if (Quest == nullptr)
	{
		return;
	}

	if (Quest->Stages.IsValidIndex(CurrentStageIndex) == false)
	{
		return;
	}

	const FP4StageDetails& CurrentStage = Quest->Stages[CurrentStageIndex];


	//이번 호출에서 실제로 진행도가 변화하였는지?
	bool bProgressChanged = false;

	// 1) 일단 일치하는 Objective에 대해 Progress 증가
	for (const FP4ObjectiveDetails& Obj : CurrentStage.Objectives)
	{
		if (Obj.ObjectiveID == ObjectiveID)
		{
			int32& CurrentValue = ObjectiveProgress.FindOrAdd(ObjectiveID);

			// 목표 수량을 넘어서 계속 올라가지 않게 하고 싶으면 if로 막기
			if (CurrentValue < Obj.Quantity)
			{
				CurrentValue++;

				bProgressChanged = true;

				UE_LOG(LogTemp, Log, TEXT("Objective %s progress: %d / %d"),
					*ObjectiveID, CurrentValue, Obj.Quantity);
			}
		}
	}

	if (bProgressChanged == true)
	{
		OnQuestUpdated.Broadcast();
	}

	//2) 스테이지 전체 완료 여부 체크.
	bool bStageComplete = true;

	for (const FP4ObjectiveDetails& Obj : CurrentStage.Objectives)
	{
		if (Obj.Quantity <= 0)
		{
			continue;
		}

		int32* ProgressPtr = ObjectiveProgress.Find(Obj.ObjectiveID);

		if (ProgressPtr == nullptr || *ProgressPtr < Obj.Quantity) //아직 퀘스트가 완료되지 않았으면.
		{
			bStageComplete = false; //스테이지 완료여부 비활성화.
			break; //루프 탈출.
		}
	}

	if (bStageComplete == false)
	{
		return; //목표치에 미달해서 스테이지 완료여부가 참이 아니면 리턴.
	}

	UE_LOG(LogTemp, Log, TEXT("Stage %d complete for Quest %d"),
		CurrentStageIndex, CurrentQuestCode);

	CurrentStageIndex++;

	// 3) 다음 스테이지가 있으면 초기화
	if (Quest->Stages.IsValidIndex(CurrentStageIndex))
	{
		ObjectiveProgress.Empty();

		const FP4StageDetails& NextStage = Quest->Stages[CurrentStageIndex];
		for (const FP4ObjectiveDetails& Obj : NextStage.Objectives)
		{
			if (!Obj.ObjectiveID.IsEmpty())
			{
				ObjectiveProgress.Add(Obj.ObjectiveID, 0);
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Move to Stage %d of Quest %d"),
			CurrentStageIndex, CurrentQuestCode);

		OnQuestUpdated.Broadcast();
	}
	else
	{
		// 4) 더 이상 스테이지 없다 → 퀘스트 완료
		UE_LOG(LogTemp, Log, TEXT("Quest %d complete!"), CurrentQuestCode);
		const int32 FinishedQuestCode = CurrentQuestCode;

		bQuestActive = false;
		CurrentQuestCode = -1;
		CurrentStageIndex = 0;
		ObjectiveProgress.Empty();

		ClearedQuestCodes.Add(FinishedQuestCode);

		OnQuestCleared.Broadcast(FinishedQuestCode);

	}
}

int32 UP4QuestManager::GetObjectiveProgress(const FString& ObjectiveID) const
{
	if (const int32* Found = ObjectiveProgress.Find(ObjectiveID))
	{
		return *Found;
	}

	return 0;
}

const FP4QuestInfo* UP4QuestManager::GetCurrentQuest() const
{
	if (bQuestActive == false || CurrentQuestCode == -1)
	{
		UE_LOG(LogTemp, Display, TEXT("현재 퀘스트중이 아닙니다."));
		return nullptr;
	}

	return GetQuest(CurrentQuestCode);
}

const FP4StageDetails* UP4QuestManager::GetCurrentStage() const
{
	const FP4QuestInfo* Quest = GetCurrentQuest();

	if (Quest == nullptr)
	{
		return nullptr;
	}

	if (Quest->Stages.IsValidIndex(CurrentStageIndex) == false)
	{
		return nullptr;
	}

	return &Quest->Stages[CurrentStageIndex];
}
