// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Mob/P4MonsterShamos.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P4MonsterProjectile.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Physics/P4Collision.h"
#include "Game/P4GameInstance.h"
#include "Kismet/GameplayStatics.h"

AP4MonsterShamos::AP4MonsterShamos()
{
	MonsterID = FName("Shamos");
	
	// Mesh 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Box Collision 설정
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(GetMesh(), TEXT("Armature_ems054_00"));
	BoxCollision->SetRelativeLocation(FVector(0.f, -64.f, 10.f));
	BoxCollision->SetBoxExtent(FVector(30.f, 65.f, 165.f));
	BoxCollision->SetCollisionProfileName(CPROPILE_P4CAPSULE);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(
		TEXT("/Game/Monster/Model/Shamos/Shamos.Shamos")
	);
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(
		TEXT("/Game/Monster/Animation/ABP_Jagras.ABP_Jagras_C")
	);
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(
		TEXT("/Game/Monster/Model/Jagras/AM_Jagras_Attack.AM_Jagras_Attack")
	);
	if (AttackActionMontageRef.Succeeded())
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamagedMontageRef(
		TEXT("/Game/Monster/Model/Jagras/AM_Jagras_Damaged.AM_Jagras_Damaged")
	);
	if (DamagedMontageRef.Succeeded())
	{
		DamagedMontage = DamagedMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Game/Monster/Model/Jagras/AM_Jagras_Dead.AM_Jagras_Dead")
	);
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AP4MonsterShamos::AttackByAI()
{
	FName MontageSectionName = TEXT("MonsterAttack");
	AttackActionBegin(MontageSectionName, 3.f);
}

void AP4MonsterShamos::AttackHitCheck()
{
	Super::AttackHitCheck();
}

void AP4MonsterShamos::SetupAttackDelegate()
{
	Super::SetupAttackDelegate();

	
	// 몽타주 섹션에 맞게 섹션 이름들 설정
	AttackSectionNames = {"MonsterAttack"};

	// 각 섹션에 맞는 공격함수 델리게이트로 바인드
	FMonsterAttackDelegate Patern1;
	Patern1.BindUObject(this, &AP4MonsterShamos::RangeAttack);

	// 바인딩한 델리게이트로 AttackDelegates 배열 설정
	AttackDelegates = {Patern1};
}

void AP4MonsterShamos::SetDead()
{
	Super::SetDead();

	// auto* GI = GetWorld()->GetGameInstance<UP4GameInstance>();
	//
	// if ((GI != nullptr) && (GI->QuestManager != nullptr))
	// {
	// 	if (GI->QuestManager->IsQuestActive() == false)
	// 	{
	// 		return;
	// 	}
	//
	// 	GI->QuestManager->UpdateObjective(TEXT("Shamos_Kill"));
	// }
}

void AP4MonsterShamos::RangeAttack()
{
	UE_LOG(LogTemp, Log, TEXT("Call DoubleWaterBomb func"));

	// 투사체 생성 위치
	FVector ProjectileStartLocation = GetMesh()->GetSocketLocation(FName("BreathPos"));
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector TargetPos = PlayerPawn->GetActorLocation();
		
		// 플레이어로 향하는 방향 벡터
		FVector Direction = TargetPos - ProjectileStartLocation;
		Direction.Normalize();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		FTransform Transform;
		Transform.SetLocation(ProjectileStartLocation);
		Transform.SetRotation(Direction.ToOrientationQuat());

		// 투사체 생성
		AP4MonsterProjectile* Projectile = GetWorld()->SpawnActor<AP4MonsterProjectile>(WaterProjectileClass, Transform, SpawnParams);
		if (Projectile)
		{
			// 초기 값 설정
			Projectile->InitProjectile(3.f, 10.f, 150.f, false);
			
			// 투사체에게 몬스터 정보를 얻기 위해서 Owner 로 설정
			Projectile->SetOwner(this);

			// 투사체 속도 초기화
			Projectile->FireInDirection(Transform.Rotator().Vector());
		}
	}
}
