// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
//#include "Interface/AnimationAttackInterface.h"
#include "Interface/P4DamageableInterface.h"
#include "Interface/P4RollingInterface.h"
#include "Interface/P4CharacterWidgetInterface.h"
#include "P4CharacterBase.generated.h"

UCLASS()
class WANTED_PROJECT4_API AP4CharacterBase : 
	public ACharacter,
	public IAbilitySystemInterface,
	public IP4DamageableInterface,
	public IP4CharacterWidgetInterface,
	public IP4RollingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AP4CharacterBase();

	// GAS 접근용
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// AttributeSet 접근용
	class UP4PlayerAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// -작성: 노현기 -일시: 2025.11.10
	// 인벤토리 컴포넌트 접근용
	FORCEINLINE class UP4InventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	// -작성: 노현기 -일시: 2025.11.14
	// 무기 컴포넌트 접근용
	FORCEINLINE class UP4WeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	// -작성: 노현기 -일시: 2025.11.18
	// 장비창 컴포넌트 접근용
	FORCEINLINE class UP4EquipmentInvenComponent* GetEquipmentInvenComponent() const { return EquipmentInvenComp; }

	// todo: 플레이어에 직접 둘지 아니면 플레이어가 오버라이드해야할지 모르겠음.
	//void SetupGASInputComponent();


public:
	// 공격 받았을 경우 처리 함수
	UFUNCTION(BlueprintCallable, Category = Monster)
	virtual void ApplyDamage(const float DamageAmount) override;

	// 공격할 시 데미지 주는 처리 함수
	UFUNCTION(BlueprintCallable, Category = Monster)
	virtual void GiveDamage(AActor* TargetActor, const float DamageAmount) override;


	void HandleRespawn();


public:
	FORCEINLINE virtual class UAnimMontage* GetDefaultAttackMontage() const { return DefaultAttackMontage; }
	// -작성: 노현기 -일시: 2025.11.19
	FORCEINLINE virtual class UAnimMontage* GetStrongAttackMontage() const { return StrongAttackMontage; }
	//FORCEINLINE virtual class UAnimMontage* GetJumpMontage() const { return JumpMontage; }
	//FORCEINLINE class UABComboActionData* GetComboActionData() const { return ComboActionData; }
	FORCEINLINE class UAnimMontage* GetDeadMontage() const { return DeadMontage; }
	
protected:
	// Damaged 몽타주 실행 및 종료 시 호출될 함수
	void DamagedActionBegin();
	void DamagedActionEnd(UAnimMontage* TargetMontage, bool Interrupted);
	// 공격, 피격 애니메이션 진행 여부를 확인하기 위한 변수

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
protected:
	// IABAnimationAttackInterface 함수 구현.
	// 애님 노티파이 기반으로 충돌 판정하는 목적으로 사용.
	//virtual void AttackHitCheck() override;

	// 몬스터가 죽었을 시 실행 될 함수
	virtual void SetDead();

	// 죽음 애니메이션 재생.
	void PlayDeadAnimation();
protected:
	// AbilitySystem
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// AttributeSet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UP4PlayerAttributeSet> AttributeSet;

	// -작성: 노현기 -일시: 2025.11.10
	// 인벤토리 컴포넌트 (컨트롤러에서 접근)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<class UP4InventoryComponent> InventoryComp;

	// -작성: 노현기 -일시: 2025.11.14
	// 무기 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WeaponComponent> WeaponComponent;

	// -작성: 노현기 -일시: 2025.11.18
	// 장비창 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4EquipmentInvenComponent> EquipmentInvenComp;

protected:
	//virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	//UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	//TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	// -작성: 노현기 -일시: 2025.11.19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StrongAttackMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UABComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DamagedMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;


	//virtual void BeginPlay() override;
public:
	FTransform RespawnTransform;


	//작성 한승헌- 일시 2025.11.19
	//타임라인 변수 모음
	//안쓸것같지만 일단 생성
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeLine)
	TObjectPtr<class UTimelineComponent> RollingTimeLine = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeLine)
	TObjectPtr<class UCurveFloat> RollingCurve = nullptr;

	FVector RollingDirection;

	virtual FVector GetRollingDirection() const override;
	virtual void SetRollingDirection(const FVector InRollingDirection) override;
};
