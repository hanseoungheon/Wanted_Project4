// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/MonsterAIInterface.h"
#include "Interface/P4DamageableInterface.h"
#include "Stat/P4MonsterAttributeSet.h"
#include "Wanted_Project4/Interface/AnimationAttackInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GameplayCueInterface.h"  // Cue 인터페이스 정의 포함
#include "P4MonsterBase.generated.h"

// -작성: 노현기 -일시: 2025.11.19
// 드롭 아이템 정보 구조체
USTRUCT(BlueprintType)
struct FMonsterDropItem
{
	GENERATED_BODY()

	// 드롭할 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UItemDataBase> ItemData;

	// 이 아이템의 최소 드롭 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinQuantity = 1;

	// 이 아이템의 최대 드롭 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity = 1;

	// 드롭 확률 (0.0 ~ 1.0, 1.0 = 100%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance = 1.0f;

	FMonsterDropItem()
		: ItemData(nullptr), MinQuantity(1), MaxQuantity(1), DropChance(1.0f)
	{
	}
};

// 몬스터 공격 시 각 공격을 실행할 델리게이트
DECLARE_DELEGATE(FMonsterAttackDelegate);

UCLASS()
class WANTED_PROJECT4_API AP4MonsterBase
	: public ACharacter,
	  public IAbilitySystemInterface,
	  public IAnimationAttackInterface,
	  public IMonsterAIInterface,
	  public IP4DamageableInterface //, public IGameplayCueInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AP4MonsterBase();

	// 인터페이스로 구현하기
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE const UP4MonsterAttributeSet* GetAttributeSet() const { return AttributeSet; }

public:
	virtual void BeginPlay() override;

	// 컴포넌트 초기화 끝나면 호출되는 함수
	virtual void PostInitializeComponents() override;

	// 애님 노티파이에서 실행할 Interface 함수 구현
	// 몬스터 공격 판정 함수
	// 현재 델리게이트 배열로 공격들 관리하면서 안쓰는 중
	virtual void AttackHitCheck() override;

public:
	// 공격 받았을 경우 처리 함수
	UFUNCTION(BlueprintCallable, Category = Monster)
	virtual void ApplyDamage(const float DamageAmount) override;

	// 공격할 시 데미지 주는 처리 함수
	UFUNCTION(BlueprintCallable, Category = Monster)
	virtual void GiveDamage(AActor* TargetActor, const float DamageAmount) override;

	//virtual void HandleGameplayCue(
	//	AActor* MyTarget,
	//	FGameplayTag EventTag,
	//	EGameplayCueEvent::Type EventType,
	//	const FGameplayCueParameters& Parameters
	//) override;

	// Monster AI Interface 구현
public:
	// BB, BT 반환
	FORCEINLINE virtual UBlackboardData* GetBBAsset() const override { return BBAsset; }
	FORCEINLINE virtual UBehaviorTree* GetBTAsset() const override { return BTAsset; }

	// AttributeSet 에 있음
	FORCEINLINE virtual float GetAIDetectRange() override { return AttributeSet->GetDetectRange(); }
	FORCEINLINE virtual float GetAIChaseRange() override { return AttributeSet->GetChaseRange(); }
	FORCEINLINE virtual float GetAITurnSpeed() override { return AttributeSet->GetTurnSpeed(); }
	
	FORCEINLINE virtual float GetAIChaseSpeed() override { return AttributeSet->GetChaseSpeed(); }
	FORCEINLINE virtual float GetAIMovementSpeed() override { return AttributeSet->GetMovementSpeed(); }

	// @Todo: AttributeSet 에 없는 애들을 일단 어떻게 할 것인가
	// AttributeSet 에 없음
	FORCEINLINE virtual float GetAIAttackRange() override { return 250.f; }
	FORCEINLINE virtual float GetAIPatrolRadius() override { return 900.f; }

	// 공격 요청 함수
	virtual void AttackByAI() override;

	// 이동속도 설정 함수
	FORCEINLINE void SetCharacterMovementSpeed(float InSpeed) override { GetCharacterMovement()->MaxWalkSpeed = InSpeed; }

	// 공격 종료 시점 델리게이트 호출 함수 (종료 시점임을 알림)
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;

	// 공격 몽타주가 끝나면 알려주는 함수
	virtual void NotifyActionEnd();

	//SetAIAttackDelegate 에서 전달된 델리게이트를 저장할 변수
	FAIMonsterAttackFinished OnAttackFinished;

	// 입력받은 섹션 네임의 공격 몽타주를 실행할 함수
	virtual void AttackActionBegin(FName& InAttackMontageSectionName, const float AttackSpeed);
	void AttackActionEnd(UAnimMontage* TargetMontage, bool Interrupted);

	// Damaged 몽타주 실행 및 종료 시 호출될 함수
	virtual void DamagedActionBegin();
	void DamagedActionEnd(UAnimMontage* TargetMontage, bool Interrupted);

	// 공격, 피격 애니메이션 진행 여부를 확인하기 위한 변수
	virtual bool GetIsDamaged() override { return IsDamaged; }
	virtual bool GetIsAttacking() override { return IsAttacking; }
	bool IsAttacking = false;
	bool IsDamaged = false;

protected:
	// 몬스터가 죽었을 시 실행 될 함수
	virtual void SetDead();

	// -작성: 노현기 -일시: 2025.11.19
	// 랜덤 아이템 드롭 함수
	void DropRandomItems();

	// ASC
	// 몬스터의 경우 일시적이므로 Character 에 붙임
protected:
	// ASC
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 몬스터 스탯 AttributeSet
	UPROPERTY()
	TObjectPtr<UP4MonsterAttributeSet> AttributeSet;

	// 몬스터 스탯을 들고올 DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UDataTable> MonsterStatData;

	// 몬스터 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	FName MonsterID;

	// 선공 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	bool bIsAgressive;

	// 애니메이션 관련
protected:
	// 몬스터 공격 애님 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	// 몬스터 피격 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DamagedMontage;

	// 몬스터 사망 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

protected:
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

	// 몬스터 공격 관리 섹션
protected:
	// 공격 델리게이트를 배열로 저장
	TArray<FMonsterAttackDelegate> AttackDelegates;

	// 공격 몽타주 섹션 이름 배열
	// 실제 섹션 배치와 이름이 동일해야 함
	UPROPERTY(EditAnywhere, Category = Attack)
	TArray<FName> AttackSectionNames;

	// 공격 델리게이트 배열을 설정 (자식이 오버라이드)
	virtual void SetupAttackDelegate();

public:
	virtual void ExecuteAttackSection(const FName& SectionName) override;

// -작성: 노현기 -일시: 2025.11.19
// 몬스터 아이템 드랍 섹션
 protected:
	  // 드롭 아이템 목록 (구조체 배열로 변경)
	  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	  TArray<FMonsterDropItem> DropItemPool;

	  // 한 번에 드롭할 아이템 종류 최소/최대 개수
	  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	  int32 MinDropItemTypes = 1;  // 최소 1종류

	  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	  int32 MaxDropItemTypes = 4;  // 최대 4종류
};
