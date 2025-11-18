// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Mob/P4MonsterProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AP4MonsterProjectile::AP4MonsterProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(15.f);
		CollisionComponent->OnComponentHit.AddDynamic(this, &AP4MonsterProjectile::OnHit);
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
			TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 1500.f;
		ProjectileMovementComponent->MaxSpeed = 2000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	}

	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		MeshComponent->SetupAttachment(RootComponent);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(
			TEXT("/Engine/BasicShapes/Sphere.Sphere")
		);
		if (StaticMeshRef.Succeeded())
		{
			MeshComponent->SetStaticMesh(StaticMeshRef.Object);
		}
	}

	// 발사체 수명 설정
}

// Called when the game starts or when spawned
void AP4MonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AP4MonsterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP4MonsterProjectile::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor,
                                 UPrimitiveComponent* _OtherPrimitiveCom, FVector _nNormalImpulse,
                                 const FHitResult& _Hit)
{
	if (_OtherActor != this && _OtherPrimitiveCom->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Log, TEXT("[Projectile Test] Hit Object : %s"), *_OtherActor->GetName());
	}
	Destroy();
}

void AP4MonsterProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AP4MonsterProjectile::Bomb()
{
}
