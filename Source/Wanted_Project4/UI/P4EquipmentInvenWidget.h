// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GameplayTagContainer.h"
#include "P4EquipmentInvenWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4EquipmentInvenWidget : public UP4CustomWidget
{
	GENERATED_BODY()
	
public:
	UP4EquipmentInvenWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

    // 소멸자에서 델리게이트 해제
    virtual void BeginDestroy() override;

    // 장비창 컴포넌트랑 바인드
    void BindEquipInven(class UP4EquipmentInvenComponent* InEquipInvenComp);

    // UI 업데이트
    void RefreshUI();

    // 특정 슬롯만 업데이트
    void RefreshSlot(FGameplayTag SlotType);


    //  마우스 이벤트 처리 함수 추가
    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    // 마우스 움직임 감지
    virtual FReply NativeOnMouseMove(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseButtonUp(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;


public:
    // 인벤토리 컴포넌트로 인벤토리 데이터 참조
    UPROPERTY()
    TObjectPtr<class UP4EquipmentInvenComponent> EquipInvenComp;

protected:
    // 무기 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> WeaponSlot;

    // 투구 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> HeadSlot;

    // 갑옷 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> ChestSlot;

    // 장갑 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> GlovesSlot;

    // 바지 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> LegsSlot;

    // 신발 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UP4Slot> ShoesSlot;
private:
    bool bIsDragging;

    // 오프셋
    FVector2D DragOffset;

    //  장비창 영역 CanvasPanel (BindWidget으로 자동 바인딩)
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> EquipPanel;

    // 인벤토리 드래그 하기 위한 헤더보드
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> DragHeader;

};
