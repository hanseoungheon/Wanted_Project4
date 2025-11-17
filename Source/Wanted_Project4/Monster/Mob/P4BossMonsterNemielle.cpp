// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Mob/P4BossMonsterNemielle.h"

#include "Monster/P4MonsterBase.h"
#include "Monster/AI/P4MonsterAIController.h"
#include "Monster/GA/P4GA_EnergyBomb.h"
#include "Monster/GA/P4GA_Howling.h"
#include "Monster/GA/P4GA_LeftWingStomp.h"

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

	// @Todo: 보스 몬스터 몽타주 생성자 초기화
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(
		TEXT("/Game/Monster/Model/Nemielle/AM_NamielleAttack.AM_NamielleAttack")
	);
	if (AttackActionMontageRef.Succeeded())
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}
}

void AP4BossMonsterNemielle::BeginPlay()
{
	Super::BeginPlay();

	// Nemielle 몬스터 패턴 설정
	TArray<FPatternData> Patterns;
	Patterns.Add({"LeftWingStomp", 30.f, 0.f, 1000.f, 1.f});
	Patterns.Add({"Howling", 30.f, 0.f, 600.f, 1.f});
	Patterns.Add({"EnergyBomb", 30.f, 0.f, 300.f, 1.f});

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
}

void AP4BossMonsterNemielle::SetupAttackDelegate()
{
	//보스 몬스터 패턴 Delegate 생성 후 넘겨주기
	Super::SetupAttackDelegate();

	// 몽타주 섹션에 맞게 섹션 이름들 설정
	AttackSectionNames = {"LeftWingStomp", "Howling", "EnergyBomb"};

	// 각 섹션에 맞는 공격함수 델리게이트로 바인드
	FMonsterAttackDelegate Patern1;
	Patern1.BindUObject(this, &AP4BossMonsterNemielle::LeftWingStomp);
	
	FMonsterAttackDelegate Patern2;
	Patern1.BindUObject(this, &AP4BossMonsterNemielle::Howling);
	
	FMonsterAttackDelegate Patern3;
	Patern1.BindUObject(this, &AP4BossMonsterNemielle::EnergyBomb);

	// 바인딩한 델리게이트로 AttackDelegates 배열 설정
	AttackDelegates = {Patern1, Patern2, Patern3};
}

void AP4BossMonsterNemielle::LeftWingStomp()
{
	UE_LOG(LogTemp, Log, TEXT("Call LeftWingStomp func"));
}

void AP4BossMonsterNemielle::Howling()
{
	UE_LOG(LogTemp, Log, TEXT("Call Howling func"));
}

void AP4BossMonsterNemielle::EnergyBomb()
{
	UE_LOG(LogTemp, Log, TEXT("Call EnergyBomb func"));
}



