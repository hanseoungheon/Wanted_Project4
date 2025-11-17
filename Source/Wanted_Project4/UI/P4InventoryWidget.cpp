// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4InventoryWidget.h"
#include "Item/ItemDataBase.h"
#include "Inventory/P4Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Widget.h"
#include "Inventory/P4InventoryTags.h"
#include "Components/CanvasPanel.h"
#include "BluePrint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "BluePrint/WidgetLayoutLibrary.h"
#include "Player/P4PlayerController.h"
#include "Components/Border.h"

UP4InventoryWidget::UP4InventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UP4InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // ✅ 최상위 위젯의 Visibility를 코드로 설정
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	EquipmentSlots.Empty();
	ConsumableSlots.Empty();

	if (EquipmentTypeSlot)
	{
        // 인덱스 카운터 추가
        int32 Index = 0; 
		for (UWidget* Child : EquipmentTypeSlot->GetAllChildren())
		{
			if (UP4Slot* SlotWidget = Cast<UP4Slot>(Child))
			{
                SlotWidget->SlotType = P4InventoryTags::Slot::Equipment;
                SlotWidget->SlotIndex = Index++;  // 슬롯 인덱스 설정
				EquipmentSlots.Add(SlotWidget);
			}
		}
	}

	if (ConsumableTypeSlot)
	{
        // 인덱스 카운터 추가
        int32 Index = 0; 
		for (UWidget* Child : ConsumableTypeSlot->GetAllChildren())
		{
			if (UP4Slot* SlotWidget = Cast<UP4Slot>(Child))
			{
                SlotWidget->SlotType = P4InventoryTags::Slot::Consumable;
                SlotWidget->SlotIndex = Index++;  // 슬롯 인덱스 설정
				ConsumableSlots.Add(SlotWidget);
			}
		}
	}

    bIsDragging = false;

    // InventoryPanel 찾기 및 디버깅 (BindWidget 사용 시)
    if (InventoryPanel)
    {
        FVector2D PanelSize = InventoryPanel->GetCachedGeometry().GetLocalSize();
    }
    else
    {
        // BindWidget이 안 되었으면 수동으로 찾기
        TArray<UWidget*> AllWidgets;
        WidgetTree->GetAllWidgets(AllWidgets);

        for (UWidget* Widget : AllWidgets)
        {
            if (UCanvasPanel* Panel = Cast<UCanvasPanel>(Widget))
            {
                // 루트가 아닌 Canvas Panel 찾기
                if (Panel != GetRootWidget())
                {
                    InventoryPanel = Panel;
                    break;
                }
            }
        }
    }

    // ✅ 디버그: 슬롯 인덱스 확인
    UE_LOG(LogTemp, Warning, TEXT("=== 장비 슬롯 초기화 ==="));
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipmentSlots[%d] → SlotIndex: %d"),
            i, EquipmentSlots[i]->SlotIndex);
    }

    UE_LOG(LogTemp, Warning, TEXT("=== 소비 슬롯 초기화 ==="));
    for (int32 i = 0; i < ConsumableSlots.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("ConsumableSlots[%d] → SlotIndex: %d"),
            i, ConsumableSlots[i]->SlotIndex);
    }
}

void UP4InventoryWidget::BindInventory(UP4InventoryComponent* InInventoryComp)
{
    if (!InInventoryComp)
    {
        UE_LOG(LogTemp, Error, TEXT("InInventoryComp가 nullptr입니다!"));
        return;

    }
    // 1. 인벤토리 컴포넌트 저장
    InventoryComp = InInventoryComp;

    // 2. 델리게이트 바인딩 (인벤토리가 변경되면 RefreshUI 자동 호출)
    InventoryComp->OnInventoryUpdated.AddUObject(this, &UP4InventoryWidget::RefreshSlot);

    // 3. 처음 한 번 UI 업데이트
    RefreshUI();
}

void UP4InventoryWidget::RefreshUI()
{

    if (!InventoryComp)
    {
        UE_LOG(LogTemp, Error, TEXT("InventoryComp가 nullptr입니다!"));
        return;
    }

    // 장비 인벤토리 갱신
    const TArray<FInventoryItem>* EquipItems = InventoryComp->GetInventoryByType(P4InventoryTags::Slot::Equipment);
    if (EquipItems)
    {
        for (int32 i = 0; i < EquipmentSlots.Num() && i < EquipItems->Num(); ++i)
        {
            EquipmentSlots[i]->SetItem((*EquipItems)[i]);
        }
    }

    // 소비 인벤토리 갱신
    const TArray<FInventoryItem>* ConsumItems = InventoryComp->GetInventoryByType(P4InventoryTags::Slot::Consumable);
    if (ConsumItems)
    {
        for (int32 i = 0; i < ConsumableSlots.Num() && i < ConsumItems->Num(); ++i)
        {
            ConsumableSlots[i]->SetItem((*ConsumItems)[i]);
        }
    }

}

void UP4InventoryWidget::RefreshSlot(FGameplayTag SlotType, int32 SlotIndex)
{
    if (!InventoryComp)
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: InventoryComp가 nullptr"));
        return;
    }

    // 해당 타입의 인벤토리 배열 가져오기
    const TArray<FInventoryItem>* TargetArray = InventoryComp->GetInventoryByType(SlotType);
    if (!TargetArray || !TargetArray->IsValidIndex(SlotIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: 잘못된 타입[%s] 또는 인덱스[%d]"), *SlotType.ToString(),
            SlotIndex);
        return;
    }

    // 해당 타입의 UI 슬롯 배열 가져오기
    TArray<TObjectPtr<UP4Slot>>* SlotArray = nullptr;

    if (SlotType == P4InventoryTags::Slot::Equipment)
    {
        SlotArray = &EquipmentSlots;
    }
    else if (SlotType == P4InventoryTags::Slot::Consumable)
    {
        SlotArray = &ConsumableSlots;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: 알 수 없는 SlotType[%s]"), *SlotType.ToString());
        return;
    }

    // UI 슬롯 배열 유효성 검사
    if (!SlotArray || !SlotArray->IsValidIndex(SlotIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: UI 슬롯 배열 인덱스[%d] 유효하지 않음"), SlotIndex);
        return;
    }

    // 해당 슬롯만 갱신
    (*SlotArray)[SlotIndex]->SetItem((*TargetArray)[SlotIndex]);

}

FReply UP4InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{  

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {

        // DragHeader를 클릭했는지 체크
        if (DragHeader && DragHeader->IsHovered())
        {
            bIsDragging = true;

            // 뷰포트 로컬 좌표로 계산
            if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InventoryPanel->Slot))
            {
                // 현재 패널의 위치 (뷰포트 로컬 좌표)
                FVector2D CurrentPanelPosition = CanvasSlot->GetPosition();

                // 마우스 위치를 뷰포트 로컬 좌표로 변환
                FVector2D MousePosViewport = InMouseEvent.GetScreenSpacePosition();
                if (GEngine && GEngine->GameViewport)
                {
                    FVector2D ViewportSize;
                    GEngine->GameViewport->GetViewportSize(ViewportSize);

                    // DPI 스케일을 고려한 마우스 위치
                    float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
                    MousePosViewport = MousePosViewport / Scale;
                }

                // 오프셋 = 마우스 위치 - 패널 위치
                DragOffset = MousePosViewport - CurrentPanelPosition;
            }
            UE_LOG(LogTemp, Log, TEXT("인벤토리 DragHeader 클릭 - Handled"));
            return FReply::Handled().CaptureMouse(TakeWidget());
        }

        // DragHeader가 아닌 인벤토리 내부를 클릭한 경우
        if (InventoryPanel && InventoryPanel->IsHovered())
        {
            UE_LOG(LogTemp, Log, TEXT("인벤토리 내부 클릭 - 게임 입력 차단"));
            return FReply::Handled();  // 게임 입력 차단만
        }
    }

    // 인벤토리 외부 클릭 - 게임 입력 허용
    UE_LOG(LogTemp, Warning, TEXT("인벤토리 외부 - Unhandled 반환"));
    return FReply::Unhandled();
}

FReply UP4InventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InventoryPanel->Slot);

        if (!CanvasSlot)
        {
            UE_LOG(LogTemp, Error, TEXT("CanvasSlot이 nullptr!"));
            return FReply::Unhandled();
        }

        // 뷰포트 로컬 좌표로 변환
        FVector2D MousePosViewport = InMouseEvent.GetScreenSpacePosition();
        if (GEngine && GEngine->GameViewport)
        {
            float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
            MousePosViewport = MousePosViewport / Scale;
        }

        // 새 위치 = 마우스 위치 - 오프셋
        FVector2D NewPosition = MousePosViewport - DragOffset;

        // ⭐ 화면 경계 제한 추가
        FVector2D PanelSize = InventoryPanel->GetCachedGeometry().GetLocalSize();
        FVector2D ViewportSize = FVector2D::ZeroVector;

        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->GetViewportSize(ViewportSize);
            float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
            ViewportSize = ViewportSize / Scale;
        }

        // 경계 제한 (0 ~ 뷰포트 크기 - 패널 크기)
        // 패널 기준이 인벤토리 패널 기준이라 그 밖에 있는 DragHead는 위로 드래그 하다보면 잘림
        // 그래서 Y값은 Min값을 DragHeader크기만큼 늘려줌
        NewPosition.X = FMath::Clamp(NewPosition.X, 0.0f, FMath::Max(0.0f, ViewportSize.X - PanelSize.X));
        NewPosition.Y = FMath::Clamp(NewPosition.Y, 30.0f, FMath::Max(0.0f, ViewportSize.Y - PanelSize.Y)); 

        CanvasSlot->SetPosition(NewPosition);


        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply UP4InventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (bIsDragging)
        {
            bIsDragging = false;
            return FReply::Handled().ReleaseMouseCapture();
        }

        // 드래그 중이 아니었지만 인벤토리 내부 클릭이었다면
        if (InventoryPanel && InventoryPanel->IsHovered())
        {
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
}



bool UP4InventoryWidget::IsMouseOverInventory() const
{
    // 위젯이 보이지 않으면 false
    if (GetVisibility() != ESlateVisibility::Visible)
    {
        return false;
    }

    // InventoryPanel이 있으면 그것을 체크, 없으면 전체 위젯 체크
    if (InventoryPanel)
    {
        bool bIsHovered = InventoryPanel->IsHovered();
        return bIsHovered;
    }

    bool bIsHovered = this->IsHovered();
    return bIsHovered;
}