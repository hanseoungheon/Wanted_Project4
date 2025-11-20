// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GamePlayEffectTypes.h"
#include "P4HpAndShieldWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4HpAndShieldWidget : public UP4CustomWidget
{
	GENERATED_BODY()
	
//public:
//	UP4HpAndShieldWidget();
//
//	void UpdateStat();
//
//	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
//
//protected:
//	// 위젯이 모두 초기화가 된 후에 호출되는 함수.
//	// 위젯을 참조할 때 생성됐다는 것을 보장 받을 수 있음.
//	virtual void NativeConstruct() override;
//
//	//Component Sections
//public:
//
//protected:
//	UPROPERTY(meta = (BindWidget))
//	TObjectPtr<class UHorizontalBox> FillBox; //체력이 차오르는 바.
//
//	UPROPERTY(meta = (BindWidget))
//	TObjectPtr<class UImage> ImgHpFill; //체력 바 이미지
//
//	UPROPERTY(meta = (BindWidget))
//	TObjectPtr<class UImage> ImgShieldFill; // 실드바 이미지
//
//	UPROPERTY(meta = (BindWidget))
//	TObjectPtr<class UImage> ImgEmptyFill; // 비어있는바 이미지.
//
//	//슬롯 캐싱용
//	UPROPERTY()
//	TObjectPtr<class UHorizontalBoxSlot> HpSlot;
//
//	UPROPERTY()
//	TObjectPtr<class UHorizontalBoxSlot> ShieldSlot;
//
//	UPROPERTY()
//	TObjectPtr<class UHorizontalBoxSlot> EmptySlot;
//
//	//Var Sections
//public:
//
//protected:
//	UPROPERTY()
//	float CurrentHp;
//
//	UPROPERTY()
//	float MaxHp;
//
//	UPROPERTY()
//	float MaxShield;
//
//	UPROPERTY()
//	float CurrentShield;
};
