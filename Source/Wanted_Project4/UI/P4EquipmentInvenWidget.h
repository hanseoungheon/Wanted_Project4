// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "P4EquipmentInvenWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4EquipmentInvenWidget : public UP4CustomWidget
{
	GENERATED_BODY()
	
public:
	UP4EquipmentInvenWidget();

    virtual void NativeConstruct() override;

    //  마우스 이벤트 처리 함수 추가
    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseButtonUp(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    // 인벤토리 위젯 참조 설정
    void SetInventoryWidget(class UP4InventoryWidget* InInventoryWidget);

private:
    //  장비창 영역 CanvasPanel (BindWidget으로 자동 바인딩)
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> EquipPanel;

    // 인벤토리 위젯 참조
    UPROPERTY()
    TObjectPtr<class UP4InventoryWidget> InventoryWidget;
};
