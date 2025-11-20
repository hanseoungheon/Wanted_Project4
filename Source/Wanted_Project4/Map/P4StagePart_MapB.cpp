// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/P4StagePart_MapB.h"
#include "NPC/P4CatWoman.h"
#include "NPC/P4NPCForestGirl.h"
#include "Monster/Mob/P4MonsterJagras.h"
#include "Components/BoxComponent.h"


AP4StagePart_MapB::AP4StagePart_MapB()
{
	//맵 먼저 지정.
	static ConstructorHelpers::FClassFinder<AActor> MapPartRef(TEXT("/Game/Map/MapB_Test.MapB_Test_C"));

	if (MapPartRef.Succeeded() == true)
	{
		MapPart = MapPartRef.Class;
	}

	//NPC 지정.
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

	//몬스터 지정
	static ConstructorHelpers::FClassFinder<AP4MonsterBase> MonsterClassRef(TEXT("/Game/Monster/Blueprint/BP_P4MonsterJagras.BP_P4MonsterJagras_C"));

	if (MonsterClassRef.Succeeded() == true)
	{
		MonsterClass = MonsterClassRef.Class;
	}

	MonsterSpawner->SetBoxExtent(FVector(3000.0f, 5000.0f, 1000.0f));

	MonsterSpawnTrigger->SetBoxExtent(FVector(5000.0f, 6000.0f, 1000.0f));
	MonsterSpawnTrigger->SetRelativeLocation(FVector(2510.0f, -290.0f, 0.0f));


	MonsterDeleteTrigger->SetBoxExtent(FVector(10000.0f, 15000.0f, 1000.0f));
	MonsterDeleteTrigger->SetRelativeLocation(FVector(2510.0f, -1660.0f, 0.0f));

	NPC1SpawnOffset = FVector(1280.f, 3200.f, -300.f); //ForestGirl 위치.
	NPC1SpawnRotation = FRotator(0.0f, 180.0f, 0.0f); //ForestGirl 각도.

	NPC2SpawnOffset = FVector(-1980.0f, -1270.0f, -220.0); //CatWoman 위치.
}

void AP4StagePart_MapB::BeginPlay()
{
	Super::BeginPlay();
}

void AP4StagePart_MapB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
