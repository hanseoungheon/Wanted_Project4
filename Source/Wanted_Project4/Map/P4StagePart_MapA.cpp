// Fill out your copyright notice in the Description page of Project Settings.


#include "P4StagePart_MapA.h"
#include "NPC/P4CatWoman.h"
#include "NPC/P4NPCForestGirl.h"
#include "Monster/Mob/P4MonsterJagras.h"
#include "Components/BoxComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Kismet/GameplayStatics.h"

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


	MonsterSpawner->SetBoxExtent(FVector(4000.0f, 4000.0f,1000.0f));
	MonsterSpawner->SetRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));

	MonsterSpawnTrigger->SetBoxExtent(FVector(5000.0f, 5000.0f, 1000.0f));

	MonsterSpawnTrigger->SetRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));

	MonsterDeleteTrigger->SetBoxExtent(FVector(12000.0f, 12000.0f, 1000.0f));
	MonsterDeleteTrigger->SetRelativeRotation(FRotator(0.0f, 60.0f, 0.0f));

	NPC1SpawnOffset = FVector(1800.f, -680.f, -95.f); //ForestGirl 위치.
	NPC2SpawnOffset = FVector(2360.f, 520.f, -500.f); //CatWoman 위치.

}

void AP4StagePart_MapA::BeginPlay()
{
	Super::BeginPlay();

	//맨 처음 시작할때 오버랩 되어있는지 확인할려했으나 BeginPlay에선 안되서 1프레임뒤에 실행.
	//타이머에 쓸 핸들.
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(
		Handle,
		this,
		&AP4StagePart_MapA::CheckBeginPlayOverlap,
		0.01f, //약 1프레임 후 위 함수 실행.
		false
	);
}

void AP4StagePart_MapA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP4StagePart_MapA::CheckBeginPlayOverlap()
{
	//맨 처음 시작할때 오버랩 되어있는지 확인하기.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	//비교적 크기가 작은 콜리전 박스로.
	bool bOverlapMonsterSpawnBox
		= MonsterSpawnTrigger->IsOverlappingActor(PlayerCharacter);

	if (bOverlapMonsterSpawnBox == true)
	{
		//몬스터 스폰.
		SpawnMonsterInTrigger();
		//NPC 스폰
		SpawnNPCs();
	}
}
