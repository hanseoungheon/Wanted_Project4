// Fill out your copyright notice in the Description page of Project Settings.


#include "P4StagePart_MapA.h"
#include "NPC/P4CatWoman.h"
#include "NPC/P4NPCForestGirl.h"
#include "Monster/Mob/P4MonsterJagras.h"
#include "Components/BoxComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"

AP4StagePart_MapA::AP4StagePart_MapA()
{
	//맵 먼저 지정.
	static ConstructorHelpers::FClassFinder<AActor> MapPartRef(TEXT("/Game/Map/MapA_Test.MapA_Test_C"));

	if (MapPartRef.Succeeded() == true)
	{
		MapPart = MapPartRef.Class;
	}
	//UStaticMeshComponent
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

	static ConstructorHelpers::FClassFinder<AP4MonsterBase> MonsterClassRef(TEXT("/Game/Monster/Blueprint/BP_P4MonsterJagras.BP_P4MonsterJagras_C"));

	if (MonsterClassRef.Succeeded() == true)
	{
		MonsterClass = MonsterClassRef.Class;
	}

	MonsterSpawnTrigger->SetBoxExtent(FVector(4000.0f, 4000.0f, 1000.0f));
	MonsterSpawnTrigger->SetRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));

	MonsterDeleteTrigger->SetBoxExtent(FVector(10000.0f, 10000.0f, 1000.0f));
	MonsterDeleteTrigger->SetRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));

	NPC1SpawnOffset = FVector(1800.f, -680.f, -95.f); //ForestGirl 위치.
	NPC2SpawnOffset = FVector(2360.f, 520.f, -500.f); //CatWoman 위치.

}

void AP4StagePart_MapA::BeginPlay()
{
	Super::BeginPlay();
	//Nav 메시 볼륨 크기.
	//NavBoundsExtent = FVector(5000.0f, 5000.0f, 1000.0f);
	//NavMeshVolume->SetActorRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));
}

void AP4StagePart_MapA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
