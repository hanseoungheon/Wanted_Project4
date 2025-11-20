// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/ObjectPool/P4ProjectilePoolSubsystem.h"

#include "Monster/Mob/P4MonsterProjectile.h"

UP4ProjectilePoolSubsystem::UP4ProjectilePoolSubsystem()
{
	// 미리 40개 만들거라고 등록해두기
	PreSpawnList.Add(AP4MonsterProjectile::StaticClass(), 10);
}

void UP4ProjectilePoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	// 에디터에서 월드 열 때는 PreSpawn 하지 않음
#if WITH_EDITOR
	if (!World->IsPlayInEditor()) // PIE가 아니라면 생략
	{
		return;
	}
#endif
	
	// 기존 액터 삭제
	for (auto& Pair : Pools)
	{
		for (AActor* Actor : Pair.Value)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy();
			}
		}
	}
	Pools.Empty();

	// 클래스 별 미리 스폰해놓기
	for (const auto& Pair : PreSpawnList)
	{
		// PreSpawnList 로 부터 클래스와 생성 갯수 받아오기
		TSubclassOf<AActor> ProjectileClass = Pair.Key;
		int32 Count = Pair.Value;

		// 클래스가 없거나 갯수가 0개 이하면 스킵
		if (!ProjectileClass || Count <= 0)
		{
			continue;
		}

		TArray<AActor*> PoolArray = Pools.FindOrAdd(ProjectileClass);

		for (int32 i = 0 ; i < Count ; ++i)
		{
			AActor* Projectile = World->SpawnActor<AActor>(ProjectileClass);

			if (Projectile)
			{
				// 비활성화 상태로 전환
				Projectile->SetActorHiddenInGame(true);
				Projectile->SetActorTickEnabled(false);
				Projectile->SetActorEnableCollision(false);

				// 화면 밖으로 이동시키기
				Projectile->SetActorLocation(FVector(0, 0, -5000));

				// 풀에 넣기
				PoolArray.Add(Projectile);

				Projectile->SetFolderPath(TEXT("ProjectilePool"));
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Pre-Spawned %d projectiles for %s"),
		   Count,
		   *ProjectileClass->GetName());
	}
}

void UP4ProjectilePoolSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

AActor* UP4ProjectilePoolSubsystem::SpawnProjectile(TSubclassOf<AActor> ProjectileClass, const FVector& Location,
	const FRotator& Rotation)
{
	if (!ProjectileClass)
	{
		return nullptr;
	}

	// 해당 클래스의 Pool 을 가져오거나 생성함
	TArray<AActor*>& PoolArray = Pools.FindOrAdd(ProjectileClass);

	AActor* Projectile = nullptr;

	// Pool 에 비활성화된 객체 있는지 확인
	for (AActor* PooledActor : PoolArray)
	{
		auto* ProjectileActor = Cast<AP4MonsterProjectile>(PooledActor);
		if (ProjectileActor && !ProjectileActor->IsPooledActive)
		{
			Projectile = PooledActor;
			break;
		}
	}

	UWorld* World = GetWorld();

	// 비활성화 된 객체가 없다면 새로 생성
	if (!Projectile)
	{
		Projectile = World->SpawnActor<AActor>(ProjectileClass, Location, Rotation);
		PoolArray.Add(Projectile);
	}

	// 활성화 시키기
	Projectile->SetActorLocation(Location);
	Projectile->SetActorRotation(Rotation);

	Projectile->SetActorHiddenInGame(false);
	Projectile->SetActorTickEnabled(true);
	Projectile->SetActorEnableCollision(true);

	auto* ProjectileActor = Cast<AP4MonsterProjectile>(Projectile);
	if (ProjectileActor)
	{
		ProjectileActor->IsPooledActive = true;
	}

	UE_LOG(LogTemp, Log, TEXT("[Object Pool] Spawn Projectile Name : %s"), *Projectile->GetName());
	
	return Projectile;
}

void UP4ProjectilePoolSubsystem::ReturnProjectile(AActor* Projectile)
{
	if (!Projectile)
	{
		return;
	}

	// 비활성화 상태로 전환
	Projectile->SetActorHiddenInGame(true);
	Projectile->SetActorTickEnabled(false);
	Projectile->SetActorEnableCollision(false);

	auto* ProjectileActor = Cast<AP4MonsterProjectile>(Projectile);
	if (ProjectileActor)
	{
		ProjectileActor->IsPooledActive = false;
	}

	// 화면 밖으로 이동시키기
	Projectile->SetActorLocation(FVector(0, 0, -5000));

	Projectile->SetOwner(nullptr);
}
