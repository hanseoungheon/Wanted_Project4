// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "P4MonsterProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class WANTED_PROJECT4_API AP4MonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4MonsterProjectile();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnProjectileOverlappedAnywhere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	// 발사체 Sphere 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<USphereComponent> CollisionComponent;

	// 발사체 이동 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TObjectPtr<UNiagaraComponent> TrailNaiagaraComponent;

protected:
	// 발사체 수명
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float LifeSpan = 3.f;

	// 발사체 데미지
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;

	// 발사체 폭발 범위
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float Radius = 0.f;

	// 폭발 여부
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	bool IsBomb = false;

	// 현재 생성된 시간
	float CurrentLifeTime = 0.f;
	
public:
	// 데이터 초기화
	void InitProjectile(float InLifeSpan, float InDamage, float InRadius, bool InIsBomb);
	
	// 발사 방향으로 발사체 속도 초기화
	void FireInDirection(const FVector& ShootDirection);

	// 최대 사거리 또는 오브젝트에 닿았을 경우 폭발
	void Bomb();
	
};
