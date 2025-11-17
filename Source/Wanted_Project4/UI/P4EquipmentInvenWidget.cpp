// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EquipmentInvenWidget.h"
#include "Components/CanvasPanel.h"
#include "UI/P4InventoryWidget.h"

UP4EquipmentInvenWidget::UP4EquipmentInvenWidget()
{
}

void UP4EquipmentInvenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // EquipPanel이 제대로 바인딩되었는지 확인
    if (!EquipPanel)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipPanel이 BindWidget으로 바인딩되지 않았습니다!"));
    }
}

void UP4EquipmentInvenWidget::SetInventoryWidget(UP4InventoryWidget* InInventoryWidget)
{
    InventoryWidget = InInventoryWidget;
}

FReply UP4EquipmentInvenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent&
    InMouseEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("🔍 장비창 NativeOnMouseButtonDown 호출됨"));

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔍 좌클릭 감지"));
        UE_LOG(LogTemp, Warning, TEXT("🔍 EquipPanel: %s"), EquipPanel ? TEXT("존재") : TEXT("nullptr"));

        if (EquipPanel)
        {
            bool bIsHovered = EquipPanel->IsHovered();
            UE_LOG(LogTemp, Warning, TEXT("🔍 EquipPanel IsHovered: %s"), bIsHovered ? TEXT("true") :
                TEXT("false"));

            if (bIsHovered)
            {
                UE_LOG(LogTemp, Log, TEXT("✅ 장비창 내부 클릭 - Handled 반환"));
                return FReply::Handled();
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("🔍 장비창 외부 - Unhandled 반환"));
    return FReply::Unhandled();
}

FReply UP4EquipmentInvenWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent&
    InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (EquipPanel && EquipPanel->IsHovered())
        {
            return FReply::Handled();
        }
    }

    return FReply::Unhandled();
}
