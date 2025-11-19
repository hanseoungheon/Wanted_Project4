// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GamePlayEffectTypes.h"
#include "P4HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4HpBarWidget : public UP4CustomWidget
{
	GENERATED_BODY()

//Function Sections
public:
	UP4HpBarWidget(const FObjectInitializer& ObjectInitializer);

	//void BindAsc(class UAbilitySystemComponent* InAsc);

	void UpdateStat();

	void UpdateHpBar();

	void UpdateShieldBar();

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:
	// 위젯이 모두 초기화가 된 후에 호출되는 함수.
	// 위젯을 참조할 때 생성됐다는 것을 보장 받을 수 있음.
	virtual void NativeConstruct() override;


	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangedData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangedData);

	virtual void OnShieldChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxShieldChanged(const FOnAttributeChangeData& Data);
//Var Sections
public:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> P4HpBar;

	UPROPERTY()
	float CurrentHp = 0.0f; //플레이어 캐릭터의 현재 체력.

	UPROPERTY()
	float MaxHp = 0.1f; //플레이어 캐릭터의 최대 체력.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> P4ShieldBar;

	float CurrentShield = 0.0f;
	float MaxShield = 0.0f;
};
