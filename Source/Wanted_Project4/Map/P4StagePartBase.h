// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P4StagePartBase.generated.h"

//전방선언.
class AP4MonsterBase;
class AP4NPCBase;

UCLASS()
class WANTED_PROJECT4_API AP4StagePartBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4StagePartBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnSpawnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnSpawnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
	//void OnDeleteTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDeleteTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//내부 함수들.
	void SpawnMapPart(); //타일 기준 맵 파츠 스폰.

	void SpawnNPCs(); //NPC 두명 스폰.

	void SpawnMonsterInTrigger(); //SpawnTirgger안에 몬스터 랜덤 스폰.

	void SpawnNavMeshBounds(); //NavMesh 생성.

public:

	//Monster Sections
protected:
	//스폰되어있는 몬스터들.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	TArray<TObjectPtr<AP4MonsterBase>> Monsters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	TSubclassOf<AP4MonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster)
	int32 MinMonsterCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster)
	int32 MaxMonsterCount = 6;

	
	//몬스터가 이미 스폰되어있는지.
	bool bMonsterSpawned = false;



	//NPC Sections
protected:
	//스폰되어있는 NPC들.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC)
	TArray<TObjectPtr<AP4NPCBase>> NPCs;

	//NPC 스폰용 클래스들
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = NPC)
	TSubclassOf<AP4NPCBase> NPCClass1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = NPC)
	TSubclassOf<AP4NPCBase> NPCClass2;

	//각 NPC 스폰위치.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FVector NPC1SpawnOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FVector NPC2SpawnOffset = FVector::ZeroVector;




	//MapSections
protected:
	//맵 파츠
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Map)
	TSubclassOf<AActor> MapPart;

	//몬스터 스폰되는 트리거.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map)
	TObjectPtr<class UBoxComponent> MonsterSpawnTrigger;

	//이 트리거를 넘어가면 몬스터가 사라짐.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map)
	TObjectPtr<class UBoxComponent> MonsterDeleteTrigger;

	//NavMeshBoundsVolume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map)
	TObjectPtr<class ANavMeshBoundsVolume> NavMeshVolume;

	bool bSpawnLocalNavMeshBounds = true;

	FVector NavBoundsExtent; //NavMesh 크기.

	

};
