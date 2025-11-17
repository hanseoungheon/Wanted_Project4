// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AI/P4MonsterPatternComponent.h"
#include "GameFramework/Character.h"
#include "Interface/MonsterAIInterface.h"
#include "Stat/P4MonsterAttributeSet.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "P4BossMonsterBase.generated.h"

/*
 * 클래스 명 : P4BossMonsterBase
 * 기능 : 보스 몬스터의 기본 기능을 담당
 * ---------------------------------------
 * 스탯, 애니메이션, AI
 * 스탯 : AttributeSet, Gameplay Effect 를 사용해서 관리 및 조정
 * AI : Tick 에서 일정 시간마다 패턴 발동 확률 계산 -> 발동 조건 충족 시 StartPattern (이동중단)
 * 패턴 : Gameplay Ability 로 각 보스 몬스터 클래스에서 생성
 */


// 몬스터 공격 시 각 공격을 실행할 델리게이트
DECLARE_DELEGATE(FBossMonsterAttackDelegate);

UCLASS()
class WANTED_PROJECT4_API AP4BossMonsterBase
	: public ACharacter,
	  public IAbilitySystemInterface,
	  public IMonsterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AP4BossMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ASC;
	}

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// MonsterAIInterface 구현
public:	
	// AI 관련해서 구현 필요한 함수
	// BB, BT 불러오기
	FORCEINLINE virtual UBlackboardData* GetBBAsset() const override { return BBAsset; }
	FORCEINLINE virtual UBehaviorTree* GetBTAsset() const override { return BTAsset; }
	
	// AttributeSet 에 있음
	FORCEINLINE virtual float GetAIDetectRange() override { return AttributeSet->GetDetectRange(); }
	FORCEINLINE virtual float GetAIChaseRange() override { return AttributeSet->GetChaseRange(); }
	FORCEINLINE virtual float GetAITurnSpeed() override { return AttributeSet->GetTurnSpeed(); }

	// @Todo: AttributeSet 에 없는 애들을 일단 어떻게 할 것인가
	// AttributeSet 에 없음
	FORCEINLINE virtual float GetAIAttackRange() override { return 250.f; }
	FORCEINLINE virtual float GetAIPatrolRadius() override { return 900.f; }

	// 공격, 피격 재생 여부
	virtual bool GetIsDamaged() override;
	virtual bool GetIsAttacking() override;
	
	// 공격 요청 함수
	virtual void AttackByAI() override;

	// 공격 종료 시점 델리게이트 호출 함수 (종료 시점임을 알림)
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;

	// 공격 몽타주가 끝나면 알려주는 함수
	virtual void NotifyActionEnd();

	//SetAIAttackDelegate 에서 전달된 델리게이트를 저장할 변수
	FAIMonsterAttackFinished OnAttackFinished;

	// 입력받은 섹션 네임의 공격 몽타주를 실행할 함수
	virtual void AttackActionBegin(FName& InAttackMontageSectionName, const float AttackSpeed);
	void AttackActionEnd(UAnimMontage* TargetMontage, bool Interrupted);
	
protected:
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

	// ASC 섹션
protected:
	// ASC
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 보스몬스터 스탯 AttributeSet
	UPROPERTY()
	TObjectPtr<UP4MonsterAttributeSet> AttributeSet;

	// 몬스터 스탯을 들고올 DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UDataTable> MonsterStatData;

	// 몬스터 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stat)
	FName MonsterID;

	// 선공 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	bool bIsAgressive;

	// 애니메이션 섹션
protected:
	// 공격 애님 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	// 몬스터 피격 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DamagedMontage;

	// 몬스터 사망 몽타주
	UPROPERTY(EditAnywhere, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;


	// 몬스터 공격 관리 섹션
protected:
	// 패턴 관리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MonsterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UP4MonsterPatternComponent> PatternComponent;

	// 공격 델리게이트를 배열로 저장
	TArray<FBossMonsterAttackDelegate> AttackDelegates;

	// 공격 몽타주 섹션 이름 배열
	// 실제 섹션 배치와 이름이 동일해야 함
	UPROPERTY(EditAnywhere, Category = Attack)
	TArray<FName> AttackSectionNames;

	// 공격 델리게이트 배열을 설정 (자식이 오버라이드)
	virtual void SetupAttackDelegate();

	// 몬스터가 패턴 발동 중인지 저장하는 변수
	bool IsPatternActive = false;

public:
	void ExecuteAttackSection(const FName& SectionName);
	FORCEINLINE void SetIsPatternActive(bool InIsPatternActive) { IsPatternActive = InIsPatternActive; }

private:
	FTimerHandle PatternCheckTimerHandle;

	void CheckPatternProbability();
};
