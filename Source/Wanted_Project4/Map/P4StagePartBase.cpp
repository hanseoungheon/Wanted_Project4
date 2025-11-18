// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/P4StagePartBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/BoxComponent.h"
#include "Monster/P4MonsterBase.h"
#include "NPC/P4NPCBase.h"

#include "NavMesh/NavMeshBoundsVolume.h"
//#include "Character/P4CharacterPlayer.h"

// Sets default values
AP4StagePartBase::AP4StagePartBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = Root;

	//몬스터 스폰 트리거 생성.
	MonsterSpawnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MonsterSpawnTrigger"));
	MonsterSpawnTrigger->SetupAttachment(Root);
	MonsterSpawnTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MonsterSpawnTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//몬스터 삭제 트리거 생성.
	MonsterDeleteTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MonsterDeleteTrigger"));
	MonsterDeleteTrigger->SetupAttachment(Root);
	MonsterDeleteTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	MonsterDeleteTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bMonsterSpawned = false;
}
// Called when the game starts or when spawned
void AP4StagePartBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Map 파츠 스폰
	//AP4StagePartBase* Spawned 
	//	= GetWorld()->SpawnActor<AP4StagePartBase>(MapPart, FVector::ZeroVector,FRotator::ZeroRotator);
	//맵 파츠 스폰
	SpawnMapPart();

	//NPC 스폰
	SpawnNPCs();

	if (MonsterSpawnTrigger != nullptr)
	{
		MonsterSpawnTrigger->OnComponentBeginOverlap.AddDynamic(this, &AP4StagePartBase::OnSpawnTriggerBeginOverlap);
		//MonsterSpawnTrigger->OnComponentEndOverlap.AddDynamic(this, &AP4StagePartBase::OnSpawnTriggerEndOverlap);s
	}

	if (MonsterDeleteTrigger != nullptr)
	{
		//MonsterDeleteTrigger->OnComponentBeginOverlap.AddDynamic(this, &AP4StagePartBase::OnDeleteTriggerBeginOverlap);
		MonsterDeleteTrigger->OnComponentEndOverlap.AddDynamic(this, &AP4StagePartBase::OnDeleteTriggerEndOverlap);
	}

	//몬스터 스폰.
	SpawnMonsterInTrigger();

	//Nav메시 스폰.
	//SpawnNavMeshBounds();
}

// Called every frame
void AP4StagePartBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AP4StagePartBase::OnSpawnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//오버랩한 액터가 플레이어인지 검사.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor != PlayerCharacter)
	{
		return;
	}

	if (bMonsterSpawned == false)
	{
		SpawnMonsterInTrigger();
	}
}


void AP4StagePartBase::OnDeleteTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//오버랩한 액터가 플레이어인지 검사.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor != PlayerCharacter)
	{
		return;
	}

	//몬스터배열 순환하면서 삭제.
	for (AP4MonsterBase* Monster : Monsters)
	{
		if (IsValid(Monster) == true)
		{
			Monster->Destroy();
		}
	}

	Monsters.Empty();

	for (AP4NPCBase* NPC : NPCs)
	{
		if (IsValid(NPC) == true)
		{
			NPC->Destroy();
		}
	}

	NPCs.Empty();

	bMonsterSpawned = false;
}

void AP4StagePartBase::SpawnMapPart()
{
	UWorld* world = GetWorld();

	if (MapPart == nullptr || world == nullptr)
	{
		return;
	}

	FActorSpawnParameters Params;

	Params.Owner = this;

	Params.SpawnCollisionHandlingOverride 
		= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 이 타일 Actor의 위치/회전 기준으로 스폰
	const FVector  Loc = GetActorLocation();
	const FRotator Rot = GetActorRotation();

	AActor* MapActor = world->SpawnActor<AActor>(MapPart, Loc, Rot, Params);

	if (MapActor != nullptr)
	{
		// 타일(이 Actor)에 붙여놓고 싶으면:
		MapActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

}

void AP4StagePartBase::SpawnNPCs()
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;

	Params.SpawnCollisionHandlingOverride 
		= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//1번 NPC 소환.
	if (NPCClass1 != nullptr)
	{
		const FVector SpawnLoc = GetActorLocation() + NPC1SpawnOffset;
		const FRotator SpawnRot = GetActorRotation() + NPC1SpawnRotation;

		AP4NPCBase* NPC1 = world->SpawnActor<AP4NPCBase>(NPCClass1, SpawnLoc, SpawnRot, Params);

		if (NPC1 != nullptr)
		{
			NPC1->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			NPCs.Add(NPC1);
		}
	}

	if (NPCClass2 != nullptr)
	{
		const FVector SpawnLoc = GetActorLocation() + NPC2SpawnOffset;
		const FRotator SpawnRot = GetActorRotation() + NPC2SpawnRotation;

		AP4NPCBase* NPC2 = world->SpawnActor<AP4NPCBase>(NPCClass2, SpawnLoc, SpawnRot, Params);
		if (NPC2 != nullptr)
		{
			NPC2->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			NPCs.Add(NPC2);
		}
	}
}

void AP4StagePartBase::SpawnMonsterInTrigger()
{
	if (bMonsterSpawned == true 
		|| MonsterSpawnTrigger == nullptr 
		|| MonsterClass == nullptr)
	{
		return;
	}

	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	//스폰 갯수 랜덤.
	const int32 Count = FMath::RandRange(MinMonsterCount, MaxMonsterCount);

	// 트리거 박스 기준으로 랜덤 위치 뽑기
	const FVector BoxOrigin = MonsterSpawnTrigger->GetComponentLocation();
	const FVector BoxExtent = MonsterSpawnTrigger->GetScaledBoxExtent();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride 
		= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Count; ++i)
	{
		const FVector RandLoc = UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);
		const FRotator RandRot = FRotator::ZeroRotator;
		AP4MonsterBase* Monster = world->SpawnActor<AP4MonsterBase>(MonsterClass, RandLoc, RandRot, Params);

		if (Monster != nullptr)
		{
			Monster->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Monsters.Add(Monster);
		}
	}


	bMonsterSpawned = true;
}

void AP4StagePartBase::SpawnNavMeshBounds()
{
	UWorld* world = GetWorld();


	if (bSpawnLocalNavMeshBounds == false || world == nullptr)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector Loc = GetActorLocation();   // 타일 중심
	const FRotator Rot = FRotator::ZeroRotator;

	ANavMeshBoundsVolume* Volume =
		world->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), Loc, Rot, Params);

	if (Volume != nullptr)
	{
		FVector DesireExtent = NavBoundsExtent;
		FVector BoxUnits = DesireExtent / 100.0f;

		Volume->SetActorScale3D(BoxUnits);

		NavMeshVolume = Volume;
	}
}
