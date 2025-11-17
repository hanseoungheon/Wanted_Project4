// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/P4NPCForestGirl.h"
//#include "NPC/P4NPCDollCat.h"

AP4ForestGirl::AP4ForestGirl()
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletaMeshRef(TEXT("/Game/NPC/ForestGirl/NPCGirl.NPCGirl"));

	if (SkeletaMeshRef.Succeeded() == true)
	{
		GetMesh()->SetSkeletalMesh(SkeletaMeshRef.Object);
	}


	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -70.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	//임시 테스트 ABP
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/NPC/ForestGirl/Animations/ABP_ForestGirl.ABP_ForestGirl_C'"));

	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	Tag_InRange = FGameplayTag::RequestGameplayTag(FName("Character.Interaction.Quest"));
	ReportObjectiveID = "Report_Jagras";
}

void AP4ForestGirl::BeginPlay()
{
	Super::BeginPlay();
	NPCQuestCode = 1;
}

void AP4ForestGirl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP4ForestGirl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP4ForestGirl::Interaction(ACharacter* Character)
{
	Super::Interaction(Character);
}


UAbilitySystemComponent* AP4ForestGirl::GetAbilitySystemComponent() const
{
	Super::GetAbilitySystemComponent();

	return nullptr;
}
