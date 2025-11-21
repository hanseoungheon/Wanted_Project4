// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/P4WeaponComponent.h"
#include "P4WeaponComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Item/ItemDataBase.h"
#include "GameFrameWork/Character.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Character/P4CharacterPlayer.h"

// Sets default values for this component's properties
UP4WeaponComponent::UP4WeaponComponent()
{

}

bool UP4WeaponComponent::EquipWeapon(UItemDataBase* WeaponData)
{
	if (!WeaponData) return false;

	if (CurrentWeaponData && CurrentWeaponData == WeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EquipWeapon] 이미 같은 무기가 장착되어 있습니다: %s"),
			*WeaponData->GetItemName().ToString());
		return false;  // 실패 - 인벤토리에서 아이템 제거 안 됨
	}

	// 기존에 다른 무기가 있으면 제거
	if (WeaponMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("무기 이미 있음"));
		DestroyWeaponMesh();
	}

	// 무기 메시 로드
	USkeletalMesh* Mesh = WeaponData->EquipmentMesh.LoadSynchronous();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("무기 메시를 로드할 수 없습니다!"));
		return false;
	}

	CurrentWeaponData = WeaponData;
	CreateWeaponMesh(Mesh);

	// 기본적으로 등에 부착
	AttachWeaponToSocket(BackSocketName);

	// 애니메이션 상태 변경
	if (AP4CharacterPlayer* Player = Cast<AP4CharacterPlayer>(GetOwner()))
	{
		if (UP4PlayerAnimInstance* P4PlayerAnimInstance = Cast<UP4PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance()))
		{
			// todo: 자동 테스트
			P4PlayerAnimInstance->CharacterState.bIsEquipped = true;
		}
	}

	return true;
}

void UP4WeaponComponent::UnequipWeapon()
{
	DestroyWeaponMesh();
	CurrentWeaponData = nullptr;
}
#include "Components/SceneComponent.h"
void UP4WeaponComponent::AttachWeaponToSocket(FName SocketName)
{
	if (!WeaponMesh) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
	if (!CharacterMesh) return;

	// 소켓에 부착
	WeaponMesh->AttachToComponent(
		CharacterMesh,
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName
	);
}

void UP4WeaponComponent::GrabWeapon()
{
	if (!WeaponMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabWeapon: 장착된 무기가 없습니다!"));
		return;
	}

	// 등 → 손으로 이동
	AttachWeaponToSocket(HandSocketName);
}

void UP4WeaponComponent::SheathWeapon()
{
	if (!WeaponMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("SheathWeapon: 장착된 무기가 없습니다!"));
		return;
	}

	// 손 → 등으로 이동
	AttachWeaponToSocket(BackSocketName);
}


void UP4WeaponComponent::CreateWeaponMesh(USkeletalMesh* Mesh)
{
	if (!Mesh) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// 무기 메시 컴포넌트 생성
	WeaponMesh = NewObject<USkeletalMeshComponent>(OwnerCharacter, USkeletalMeshComponent::StaticClass());
	WeaponMesh->SetSkeletalMesh(Mesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCastShadow(true);
	WeaponMesh->RegisterComponent();
}

void UP4WeaponComponent::DestroyWeaponMesh()
{
	if (WeaponMesh)
	{
		WeaponMesh->DestroyComponent();
		WeaponMesh = nullptr;
	}
}

