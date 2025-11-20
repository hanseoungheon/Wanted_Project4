// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "P4ProjectilePoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4ProjectilePoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UP4ProjectilePoolSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	AActor* SpawnProjectile(TSubclassOf<AActor> ProjectileClass, const FVector& Location, const FRotator& Rotation);
	void ReturnProjectile(AActor* Projectile);

private:
	TMap<TSubclassOf<AActor>, TArray<AActor*>> Pools;

	// 클래스 별 미리 스폰할 초기 생성 개수 맵
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AActor>, int32> PreSpawnList;
};
