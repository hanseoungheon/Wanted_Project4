// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "P4MonsterAttributeSet.generated.h"

// 체력이 다 소진되어 죽음 상태 시 발행할 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// 그로기 수치 Max 시 실행할 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnGroggyStartDelegate);

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4MonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()


public:
	UP4MonsterAttributeSet();

	// PROPERTY_GETTER : GameplayEffect 에서 쓸 Attribute 핸들 생성
	// GETTER : FGameplayAttributeData 안의 값을 가져옴
	// SETTER : FGameplayAttributeData 안의 값을 수정
	// INITTER : 기본 값 설정
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, MaxHP);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, CurHP);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, DamageAmount);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, DetectRange);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, ChaseRange);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, MovementSpeed);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, ChaseSpeed);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, TurnSpeed);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, Attack);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS_BASIC(UP4MonsterAttributeSet, GroggyGauge);

protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	// 발행할 이벤트 델리게이트
	FOnHpZeroDelegate OnHpZero;
	FOnGroggyStartDelegate OnGroggyStart;
	
public:
	// 스탯=======================================================
	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHP;

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CurHP;

	// 받아야할 데미지(받는 데미지 처리용)
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData DamageAmount;

	// 범위=======================================================
	// 탐지 범위
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData DetectRange;

	// 추적 범위
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ChaseRange;

	// 속도=======================================================
	// 기본 속도 : Idle 에서 주변 돌아다니는 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MovementSpeed;
	
	// 추적 속도 : Player 를 Chase 할 때 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ChaseSpeed;
	
	// 회전 속도 : 공격 시 회전하는 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData TurnSpeed;

	// 공격=======================================================
	// 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Attack;
	
	// 공격 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AttackSpeed;

	// 그로기======================================================
	UPROPERTY(BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData GroggyGauge;
};
