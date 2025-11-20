// Fill out your copyright notice in the Description page of Project Settings.


#include "P4MonsterJagras.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Physics/P4Collision.h"
#include "Game/P4GameInstance.h"

AP4MonsterJagras::AP4MonsterJagras()
{
	MonsterID = FName("Jagras");
	
	// Mesh 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Box Collision 설정
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(GetMesh(), TEXT("Armature_ems049_00"));
	BoxCollision->SetRelativeLocation(FVector(0.f, -64.f, 10.f));
	BoxCollision->SetBoxExtent(FVector(30.f, 65.f, 165.f));
	BoxCollision->SetCollisionProfileName(CPROPILE_P4CAPSULE);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(
		TEXT("/Game/Monster/Model/Jagras/Jagras.Jagras")
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

void AP4MonsterJagras::AttackByAI()
{
	FName MontageSectionName = TEXT("MonsterAttack");
	AttackActionBegin(MontageSectionName, 3.f);
}

void AP4MonsterJagras::AttackHitCheck()
{
	Super::AttackHitCheck();

	// 현재 공격 몽타주 갖고오기

	// 섹션 이름 받아오기

	// 섹션 이름에 따른 각 패턴 별 판정 함수 호출
}

void AP4MonsterJagras::SetupAttackDelegate()
{
	Super::SetupAttackDelegate();

	// 몽타주 섹션에 맞게 섹션 이름들 설정
	AttackSectionNames = {"MonsterAttack"};

	// 각 섹션에 맞는 공격함수 델리게이트로 바인드
	FMonsterAttackDelegate Patern1;
	Patern1.BindUObject(this, &AP4MonsterJagras::MeleeAttack);

	// 바인딩한 델리게이트로 AttackDelegates 배열 설정
	AttackDelegates = {Patern1};
}

void AP4MonsterJagras::SetDead()
{
	Super::SetDead();

	//-작성: 한승헌
	//-일시: 2025.11.13
	//-내용: 퀘스트 시스템을 제작하여 테스트용으로 작성합니다.
	auto* GI = GetWorld()->GetGameInstance<UP4GameInstance>();

	if ((GI != nullptr) && (GI->QuestManager != nullptr))
	{
		if (GI->QuestManager->IsQuestActive() == false)
		{
			return;
		}

		GI->QuestManager->UpdateObjective(TEXT("Jagras_Kill"));
	}
}

void AP4MonsterJagras::MeleeAttack()
{
	FVector Start =
		GetActorLocation() +
		GetActorForwardVector() *
		(BoxCollision->GetScaledBoxExtent().Z + BoxCollision->GetRelativeLocation().Z);

	// 공격 범위 설정
	// @Todo: 나중에 변수 값으로 변경하기
	const float AttackRadius = 50.f;
	const float AttackRange = GetAIAttackRange();
	FVector End = Start + GetActorForwardVector() * AttackRadius;


	// 자신은 판정 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 몬스터 공격 범위 디버그 표시
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	float CapsuleHalfHeight = AttackRange * 0.5f;

	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		AttackRadius,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		5.0f
	);
#endif
	

	if (HitDetected)
	{
		// @MobTODO: 몬스터 충돌 판정 확인용
		UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트: %s"), *OutHitResult.GetActor()->GetName());
		
		// 다른 액터가 공격 당했을 시 처리
		GiveDamage(OutHitResult.GetActor(), AttributeSet->GetAttack());
	}
	else
	{
		// @MobTODO: 몬스터 충돌 판정 확인용
		UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트가 없습니다."));
	}
}
