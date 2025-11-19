// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Mob/P4BossMonsterNemielle.h"

#include "P4MonsterProjectile.h"
#include "Character/P4CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "Monster/P4MonsterBase.h"
#include "Monster/AI/P4MonsterAIController.h"
#include "Monster/GA/P4GA_DashAttack.h"
#include "Monster/GA/P4GA_DoubleWaterBomb.h"
#include "Monster/GA/P4GA_EnergyBomb.h"
#include "Monster/GA/P4GA_Howling.h"
#include "Monster/GA/P4GA_LeftWingStomp.h"
#include "Player/P4PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AP4BossMonsterNemielle::AP4BossMonsterNemielle()
{
	MonsterID = FName("Nemielle");

	// Mesh 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(.8f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(
		TEXT("/Game/Monster/Model/Nemielle/Nemielle.Nemielle")
	);
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(
		TEXT("/Game/Monster/Animation/ABP_Namielle.ABP_Namielle_C")
	);
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// 보스 몬스터 패턴 데이터테이블 불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable> PatternDataRef(
		TEXT("/Game/Monster/Data/NemiellePatternData.NemiellePatternData")
	);
	if (PatternDataRef.Succeeded())
	{
		MonsterPatternData = PatternDataRef.Object;
	}

	// @Todo: 보스 몬스터 몽타주 생성자 초기화
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(
		TEXT("/Game/Monster/Model/Nemielle/AM_NamielleAttack.AM_NamielleAttack")
	);
	if (AttackActionMontageRef.Succeeded())
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Game/Monster/Model/Nemielle/AM_NemielleDead.AM_NemielleDead")
	);
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AP4BossMonsterNemielle::BeginPlay()
{
	Super::BeginPlay();

	// Nemielle 몬스터 패턴 설정
	TArray<FPatternData> Patterns;

	// 데이터 테이블로 패턴 데이터 초기화 진행
	TArray<FPatternData*> AllRows;
	MonsterPatternData->GetAllRows(TEXT("Pattern Initialization"), AllRows);

	for (auto Row : AllRows)
	{
		if (!Row)
		{
			continue;
		}

		Patterns.Add(*Row);
	}

	// 설정한 패턴으로 패턴 컴포넌트의 패턴 초기화
	PatternComponent->InitializePatterns(Patterns);


	// 몬스터 스킬 어빌리티 태그 설정
	FGameplayAbilitySpec Spec1(UP4GA_LeftWingStomp::StaticClass());
	Spec1.GetDynamicSpecSourceTags().AddTag(
		FGameplayTag::RequestGameplayTag(FName("Monster.Action.LeftWingStomp"))
	);
	ASC->GiveAbility(Spec1);

	FGameplayAbilitySpec Spec2(UP4GA_Howling::StaticClass());
	Spec2.GetDynamicSpecSourceTags().AddTag(
		FGameplayTag::RequestGameplayTag(FName("Monster.Action.Howling"))
	);
	ASC->GiveAbility(Spec2);

	FGameplayAbilitySpec Spec3(UP4GA_EnergyBomb::StaticClass());
	Spec3.GetDynamicSpecSourceTags().AddTag(
		FGameplayTag::RequestGameplayTag(FName("Monster.Action.EnergyBomb"))
	);
	ASC->GiveAbility(Spec3);

	FGameplayAbilitySpec Spec4(UP4GA_DashAttack::StaticClass());
	Spec4.GetDynamicSpecSourceTags().AddTag(
		FGameplayTag::RequestGameplayTag(FName("Monster.Action.DashAttack"))
	);
	ASC->GiveAbility(Spec4);
	
	FGameplayAbilitySpec Spec5(UP4GA_DoubleWaterBomb::StaticClass());
	Spec5.GetDynamicSpecSourceTags().AddTag(
		FGameplayTag::RequestGameplayTag(FName("Monster.Action.DoubleWaterBomb"))
	);
	ASC->GiveAbility(Spec5);
}

void AP4BossMonsterNemielle::SetupAttackDelegate()
{
	//보스 몬스터 패턴 Delegate 생성 후 넘겨주기
	Super::SetupAttackDelegate();

	// 몽타주 섹션에 맞게 섹션 이름들 설정
	AttackSectionNames = {"LeftWingStomp", "Howling", "EnergyBomb", "DashAttack", "DoubleWaterBomb"};

	// 각 섹션에 맞는 공격함수 델리게이트로 바인드
	FMonsterAttackDelegate Pattern1;
	Pattern1.BindUObject(this, &AP4BossMonsterNemielle::LeftWingStomp);

	FMonsterAttackDelegate Pattern2;
	Pattern2.BindUObject(this, &AP4BossMonsterNemielle::Howling);

	FMonsterAttackDelegate Pattern3;
	Pattern3.BindUObject(this, &AP4BossMonsterNemielle::EnergyBomb);

	FMonsterAttackDelegate Pattern4;
	Pattern4.BindUObject(this, &AP4BossMonsterNemielle::DashAttack);
	
	FMonsterAttackDelegate Pattern5;
	Pattern5.BindUObject(this, &AP4BossMonsterNemielle::DoubleWaterBomb);

	// 바인딩한 델리게이트로 AttackDelegates 배열 설정
	AttackDelegates = {Pattern1, Pattern2, Pattern3, Pattern4, Pattern5};
}

void AP4BossMonsterNemielle::LeftWingStomp()
{
	UE_LOG(LogTemp, Log, TEXT("Call LeftWingStomp func"));
	// 패턴 데이터 불러오기
	FPatternData Pattern = PatternComponent->GetPatterns("LeftWingStomp");

	const float AttackRange = Pattern.AttackRange;

	FVector Start =
		GetActorLocation() +
		GetActorForwardVector() *
		AttackRange;

	const float AttackRadius = Pattern.AttackRadius;

	// 자신은 판정 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	TArray<FOverlapResult> OutHitResults;
	bool HitDetected = GetWorld()->OverlapMultiByChannel(
		OutHitResults,
		Start,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeBox(FVector(AttackRadius, AttackRadius, AttackRadius)),
		Params
	);

	// 몬스터 공격 범위 디버그 표시
#if ENABLE_DRAW_DEBUG
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugBox(
		GetWorld(),
		Start,
		FVector(AttackRadius, AttackRadius, AttackRadius),
		FRotationMatrix::MakeFromX(GetActorForwardVector()).ToQuat(),
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
				UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트: %s"), *OutHitResult.GetActor()->GetName());

				// 다른 액터가 공격 당했을 시 처리
				GiveDamage(OutHitResult.GetActor(), AttributeSet->GetAttack());
			}
		}
	}
	else
	{
		// @MobTODO: 몬스터 충돌 판정 확인용
		UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트가 없습니다."));
	}
}

void AP4BossMonsterNemielle::Howling()
{
	UE_LOG(LogTemp, Log, TEXT("Call Howling func"));
}

void AP4BossMonsterNemielle::EnergyBomb()
{
	UE_LOG(LogTemp, Log, TEXT("Call EnergyBomb func"));
	// 패턴 데이터 불러오기
	FPatternData Pattern = PatternComponent->GetPatterns("EnergyBomb");
	const float AttackRange = Pattern.AttackRange;

	FVector Start =
		GetActorLocation() +
		GetActorForwardVector() *
		AttackRange;

	const float AttackRadius = Pattern.AttackRadius;

	// 자신은 판정 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	TArray<FOverlapResult> OutHitResults;
	bool HitDetected = GetWorld()->OverlapMultiByChannel(
		OutHitResults,
		Start,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 몬스터 공격 범위 디버그 표시
#if ENABLE_DRAW_DEBUG
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugSphere(
		GetWorld(),
		Start,
		AttackRadius,
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
				UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트: %s"), *OutHitResult.GetActor()->GetName());

				// 다른 액터가 공격 당했을 시 처리
				GiveDamage(OutHitResult.GetActor(), AttributeSet->GetAttack());
			}
		}
	}
	else
	{
		// @MobTODO: 몬스터 충돌 판정 확인용
		UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트가 없습니다."));
	}
}

void AP4BossMonsterNemielle::DashAttack()
{
	// Dash Start 노티파이, MonsterDashCheck 노티파이 State에서 처리
}

void AP4BossMonsterNemielle::DoubleWaterBomb()
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
			Projectile->InitProjectile(3.f, 20.f, 150.f, true);
			
			// 투사체에게 몬스터 정보를 얻기 위해서 Owner 로 설정
			Projectile->SetOwner(this);

			// 투사체 속도 초기화
			Projectile->FireInDirection(Transform.Rotator().Vector());
		}
	}
}
