// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/P4StagePart_MapC.h"
#include "NPC/P4CatWoman.h"
#include "NPC/P4NPCForestGirl.h"
#include "Monster/Mob/P4MonsterJagras.h"
#include "Components/BoxComponent.h"

AP4StagePart_MapC::AP4StagePart_MapC()
{
	//맵 먼저 할당.
	static ConstructorHelpers::FClassFinder<AActor> MapPartRef(TEXT("/Game/Map/MapC_Test.MapC_Test_C"));

	if (MapPartRef.Succeeded() == true)
	{
		MapPart = MapPartRef.Class;
	}

	//NPC 할당.
	static ConstructorHelpers::FClassFinder<AP4NPCBase> NPCClass1Ref(TEXT("/Game/Blueprint/BP_NPC_ForestGirl.BP_NPC_ForestGirl_C"));

	if (NPCClass1Ref.Succeeded() == true)
	{
		NPCClass1 = NPCClass1Ref.Class; //NPC1이 퀘스트 NPC
	}

	static ConstructorHelpers::FClassFinder<AP4NPCBase> NPCClass2Ref(TEXT("/Game/Blueprint/BP_NPC_CatWoman.BP_NPC_CatWoman_C"));

	if (NPCClass2Ref.Succeeded() == true)
	{
		NPCClass2 = NPCClass2Ref.Class;//NPC2가 무기강화 NPC
	}


	//몬스터 할당.
	static ConstructorHelpers::FClassFinder<AP4MonsterBase> MonsterClassRef(TEXT("/Game/Monster/Blueprint/BP_P4MonsterJagras.BP_P4MonsterJagras_C"));

	if (MonsterClassRef.Succeeded() == true)
	{
		MonsterClass = MonsterClassRef.Class; //자그라스.
	}

	MonsterSpawner->SetBoxExtent(FVector(6000.0f, 6000.0f, 1000.0f));
	MonsterSpawner->SetRelativeLocationAndRotation(FVector(-5099.9f, 532.5f, 0.0f),FRotator(0.0f, 10.0f, 0.0f));

	MonsterSpawnTrigger->SetBoxExtent(FVector(7000.0f, 5000.0f, 2000.0f));
	MonsterSpawnTrigger->SetRelativeLocationAndRotation(FVector(-4882.9f, 827.1f, 0), FRotator(0.0f, 10.0f, 0.0f));

	MonsterDeleteTrigger->SetBoxExtent(FVector(12500.0f, 12500.0f, 2000.0f));
	MonsterDeleteTrigger->SetRelativeLocationAndRotation(FVector(-5099.9f, 532.5f, 0.0f), FRotator(0.0f, 10.0f, 0.0f));

	NPC1SpawnOffset = FVector(-1845.0f, 2631.0f, -1298.0f);

	NPC2SpawnOffset = FVector(-5658.0f, 1579.0f, -411.0f);
}

void AP4StagePart_MapC::BeginPlay()
{
	Super::BeginPlay();
}
