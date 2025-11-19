// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Mob/P4MonsterProjectile.h"

#include "Character/P4CharacterBase.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Monster/P4BossMonsterBase.h"
#include "Monster/P4MonsterBase.h"

// Sets default values
AP4MonsterProjectile::AP4MonsterProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!MeshComponent)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		MeshComponent->SetupAttachment(RootComponent);
		
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		MeshComponent->SetNotifyRigidBodyCollision(true);
		MeshComponent->OnComponentHit.AddDynamic(this, &AP4MonsterProjectile::OnProjectileHit);
		
		MeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(
			TEXT("/Engine/BasicShapes/Sphere.Sphere")
		);
		if (StaticMeshRef.Succeeded())
		{
			MeshComponent->SetStaticMesh(StaticMeshRef.Object);
		}
		RootComponent = MeshComponent;
	}
	
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->SetupAttachment(RootComponent);
		CollisionComponent->InitSphereRadius(55.f);
		CollisionComponent->SetCollisionProfileName(TEXT("P4Projectile"));
		CollisionComponent->SetGenerateOverlapEvents(true);
		
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AP4MonsterProjectile::OnProjectileOverlappedAnywhere);
	}

	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
			TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
		ProjectileMovementComponent->InitialSpeed = 1500.f;
		ProjectileMovementComponent->MaxSpeed = 2000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	}


	// 발사체 수명 설정
}

// Called when the game starts or when spawned
void AP4MonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	CurrentLifeTime = 0.f;
}

// Called every frame
void AP4MonsterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentLifeTime += DeltaTime;

	if (CurrentLifeTime >= LifeSpan)
	{
		if (IsBomb)
		{
			Bomb();
		}
		Destroy();
	}
}

void AP4MonsterProjectile::OnProjectileOverlappedAnywhere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Owner && OtherActor != this && OtherActor != Owner && OtherComp)
	{
		UE_LOG(LogTemp, Log, TEXT("[Projectile Test] Overlapped Object : %s"), *OtherActor->GetName());
		if (IsBomb)
		{
			Bomb();
		}
		else
		{
			AP4CharacterBase* Player = Cast<AP4CharacterBase>(OtherActor);
			if (Player)
			{
				// 다른 액터가 공격 당했을 시 처리
				// 생성 시 Monster 를 Owner 로 설정해줌
				AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(GetOwner());
				if (Monster)
				{
					// 몬스터의 GiveDamage 함수 호출
					UE_LOG(LogTemp, Log, TEXT("[Projectile Test] Damage : %f"), Damage);
					Monster->GiveDamage(Player, Damage);
				}
			}
		}
		
		Destroy();
	}
}

void AP4MonsterProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("[Projectile Test] Hit Object : %s"), *OtherActor->GetName());
	if (IsBomb)
	{
		Bomb();
	}
	Destroy();
}

void AP4MonsterProjectile::InitProjectile(float InLifeSpan, float InDamage, float InRadius, bool InIsBomb)
{
	LifeSpan = InLifeSpan;
	Damage = InDamage;
	Radius = InRadius;
	IsBomb = InIsBomb;
}

void AP4MonsterProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AP4MonsterProjectile::Bomb()
{
	FVector BombLocation = GetActorLocation();
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	
	TArray<FOverlapResult> OutHitResults;
	bool HitDetected = GetWorld()->OverlapMultiByChannel(
		OutHitResults,
		BombLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius),
		Params
	);
	
#if ENABLE_DRAW_DEBUG
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Orange;

	DrawDebugSphere(
		GetWorld(),
		BombLocation,
		Radius,
		16,
		DrawColor,
		false,
		5.f
	);
#endif
	
	if (HitDetected)
	{
		for (auto& OutHitResult : OutHitResults)
		{
			AP4CharacterBase* Player = Cast<AP4CharacterBase>(OutHitResult.GetActor());
			if (Player)
			{
				// @MobTODO: 몬스터 충돌 판정 확인용
				UE_LOG(LogTemp, Log, TEXT("투사체 폭발 시 충돌된 오브젝트: %s"), *OutHitResult.GetActor()->GetName());

				// 다른 액터가 공격 당했을 시 처리
				AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(GetOwner());
				if (Monster)
				{
					// 몬스터의 GiveDamage 함수 호출
					UE_LOG(LogTemp, Log, TEXT("[Projectile Test] Damage : %f"), Damage);
					Monster->GiveDamage(Player, Damage);
				}
			}
		}
	}
	else
	{
		// @MobTODO: 몬스터 충돌 판정 확인용
		UE_LOG(LogTemp, Log, TEXT("투사체 공격 시 충돌된 오브젝트가 없습니다."));
	}
}
