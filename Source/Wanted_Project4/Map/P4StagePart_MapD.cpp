// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/P4StagePart_MapD.h"
#include "NPC/P4CatWoman.h"
#include "NPC/P4NPCForestGirl.h"
#include "Monster/Mob/P4MonsterJagras.h"
#include "Components/BoxComponent.h"

AP4StagePart_MapD::AP4StagePart_MapD()
{
	//맵 가져오기.
	static ConstructorHelpers::FClassFinder<AActor> MapPartRef(TEXT("/Game/Map/MapD_Test.MapD_Test_C"));

	if (MapPartRef.Succeeded() == true)
	{
		MapPart = MapPartRef.Class;
	}

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

	//MonsterSpawnTrigger->SetBoxExtent(FVector(4000.0f, 3000.0f, 1000.0f));
	MonsterSpawner->SetBoxExtent(FVector(4000.0f, 3000.0f, 1000.0f));
	
	MonsterDeleteTrigger->SetBoxExtent(FVector(10000.0f, 6000.0f, 1000.0f));

	//ForestGirl 위치 설정.
	NPC1SpawnOffset = FVector(730.0f, -2790.0f, 390.0f);
	NPC1SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);

	//CatWoman 위치 설정.
	NPC2SpawnOffset = FVector(-110.0f, -2810.0f, 380.0f);
	NPC2SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);
}

void AP4StagePart_MapD::BeginPlay()
{
	Super::BeginPlay();
}

void AP4StagePart_MapD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
