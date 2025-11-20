// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GamePlayEffectTypes.h"
#include "P4BossHpBar.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4BossHpBar : public UP4CustomWidget
{
	GENERATED_BODY()
	
public:
	UP4BossHpBar();

	void UpdateHpBar();

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:
	// 위젯이 모두 초기화가 된 후에 호출되는 함수.
	// 위젯을 참조할 때 생성됐다는 것을 보장 받을 수 있음.
	virtual void NativeConstruct() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangedData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangedData);
public:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> P4MonsterHpBar;

	UPROPERTY()
	//몬스터의 현재 체력.
	float MonsterCurrentHp = 0.0f;

	UPROPERTY()
	//몬스터의 최대 체력.
	float MonsterMaxHp = 0.1f; 

};
