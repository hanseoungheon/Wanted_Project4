// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/P4CatWoman.h"
#include "UI/P4EnchantWidget.h"
#include "Character/P4CharacterPlayer.h"
#include "Game/P4UpgradeType.h"
#include "Game/P4UpgradeType.h"
AP4CatWoman::AP4CatWoman()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Game/NPC/DollCat/DollCat.DollCat"));

	if (SkeletalMeshRef.Succeeded() == true)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -90.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/NPC/DollCat/Animations/ABP_NPCTest.ABP_NPCTest_C"));

	if (AnimInstanceRef.Succeeded() == true)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	Tag_InRange = FGameplayTag::RequestGameplayTag(FName("Character.Interaction.Enchant"));

	TAG_Data_Weapon_AttackRate 
		= FGameplayTag::RequestGameplayTag(FName("Data.Weapon.Attack"));

	TAG_Data_Weapon_MaxHealths 
		= FGameplayTag::RequestGameplayTag(FName("Data.Weapon.MaxHealth"));
}

void AP4CatWoman::BeginPlay()
{

}

void AP4CatWoman::Tick(float DeltaTime)
{
}

void AP4CatWoman::HandleEnchantWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("CatWoman::HandleEnchantWeapon called"));
	const float BonusAttackRate = 10.0f;
	const float BonusMaxHelath = 20.0f;
	//EP4UpgradeType UpgradeType = EP4UpgradeType::Attack;
	AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(OverlapedActor);

	if (PlayerCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CatWoman: Found player, applying enchant"));
		PlayerCharacter->ApplyEnchantWeapon(BonusAttackRate,EP4UpgradeType::Attack);
	}
}
