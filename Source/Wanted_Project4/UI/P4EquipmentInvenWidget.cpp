// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EquipmentInvenWidget.h"
#include "Components/CanvasPanel.h"
#include "UI/P4InventoryWidget.h"
#include "Inventory/P4Slot.h"
#include "Player/P4PlayerController.h"
#include "BluePrint/WidgetTree.h"
#include "BluePrint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Inventory/P4InventoryTags.h"
#include "Inventory/P4EquipmentInvenComponent.h"

UP4EquipmentInvenWidget::UP4EquipmentInvenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UP4EquipmentInvenWidget::BindEquipInven(UP4EquipmentInvenComponent* InEquipInvenComp)
{
    if (!InEquipInvenComp)
    {
        UE_LOG(LogTemp, Error, TEXT("InEquipInvenComp가 nullptr입니다!"));
        return;
    }

    // 1. 컴포넌트 저장
    EquipInvenComp = InEquipInvenComp;

    // 2. 델리게이트 바인딩 (장비가 변경되면 RefreshSlot 자동 호출)
    EquipInvenComp->OnEquipmentUpdated.AddUObject(this, &UP4EquipmentInvenWidget::RefreshSlot);

    // 3. 처음 한 번 UI 업데이트
    RefreshUI();
}

void UP4EquipmentInvenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 최상위 위젯의 Visibility를 코드로 설정
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    // 슬롯에 태그 부여 + 장비창 슬롯임을 표시
    if (WeaponSlot)
    {
        WeaponSlot->SlotType = P4InventoryTags::Slot::Equipments::Weapon;
        WeaponSlot->bIsEquipmentSlot = true;
    }

    if (HeadSlot)
    {
        HeadSlot->SlotType = P4InventoryTags::Slot::Equipments::Head;
        HeadSlot->bIsEquipmentSlot = true; 
    }

    if (ChestSlot)
    {
        ChestSlot->SlotType = P4InventoryTags::Slot::Equipments::Chest;
        ChestSlot->bIsEquipmentSlot = true;  
    }

    if (GlovesSlot)
    {
        GlovesSlot->SlotType = P4InventoryTags::Slot::Equipments::Gloves;
        GlovesSlot->bIsEquipmentSlot = true; 
    }

    if (LegsSlot)
    {
        LegsSlot->SlotType = P4InventoryTags::Slot::Equipments::Legs;
        LegsSlot->bIsEquipmentSlot = true; 
    }

    if (ShoesSlot)
    {
        ShoesSlot->SlotType = P4InventoryTags::Slot::Equipments::Shoes;
        ShoesSlot->bIsEquipmentSlot = true; 
    }

    // EquipPanel이 제대로 바인딩되었는지 확인
    if (!EquipPanel)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipPanel이 BindWidget으로 바인딩되지 않았습니다!"));
    }

    bIsDragging = false;

    // EquipPanel 찾기 및 디버깅 (BindWidget 사용 시)
    if (EquipPanel)
    {
        FVector2D PanelSize = EquipPanel->GetCachedGeometry().GetLocalSize();
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
                    EquipPanel = Panel;
                    break;
                }
            }
        }
    }
}

void UP4EquipmentInvenWidget::BeginDestroy()
{
    // 델리게이트 바인딩 해제
    if (EquipInvenComp)
    {
        EquipInvenComp->OnEquipmentUpdated.RemoveAll(this);
        UE_LOG(LogTemp, Log, TEXT("[EquipmentInvenWidget] 델리게이트 바인딩 해제"));
    }

    Super::BeginDestroy();
}

void UP4EquipmentInvenWidget::RefreshUI()
{
    if (!EquipInvenComp)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipInvenComp가 nullptr입니다!"));
        return;
    }

    // 무기 슬롯
    if (WeaponSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Weapon);
        if (Item && Item->ItemData != nullptr)  // 포인터 체크 + ItemData 체크
        {
            WeaponSlot->SetItem(*Item);
        }
        else
        {
            WeaponSlot->ClearSlot();  // 빈 슬롯으로
        }
    }

    // 투구 슬롯
    if (HeadSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Head);
        if (Item && Item->ItemData != nullptr)
        {
            HeadSlot->SetItem(*Item);
        }
        else
        {
            HeadSlot->ClearSlot();
        }
    }

    // 갑옷 슬롯
    if (ChestSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Chest);
        if (Item && Item->ItemData != nullptr)
        {
            ChestSlot->SetItem(*Item);
        }
        else
        {
            ChestSlot->ClearSlot();
        }
    }

    // 장갑 슬롯
    if (GlovesSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Gloves);
        if (Item && Item->ItemData != nullptr)
        {
            GlovesSlot->SetItem(*Item);
        }
        else
        {
            GlovesSlot->ClearSlot();
        }
    }

    // 바지 슬롯
    if (LegsSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Legs);
        if (Item && Item->ItemData != nullptr)
        {
            LegsSlot->SetItem(*Item);
        }
        else
        {
            LegsSlot->ClearSlot();
        }
    }

    // 신발 슬롯
    if (ShoesSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(P4InventoryTags::Slot::Equipments::Shoes);
        if (Item && Item->ItemData != nullptr)
        {
            ShoesSlot->SetItem(*Item);
        }
        else
        {
            ShoesSlot->ClearSlot();
        }
    }

}

void UP4EquipmentInvenWidget::RefreshSlot(FGameplayTag SlotType)
{
    if (!EquipInvenComp)
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: EquipInvenComp가 nullptr"));
        return;
    }

    // 해당 슬롯 타입에 맞는 UI 슬롯 찾기
    UP4Slot* TargetSlot = nullptr;

    if (SlotType == P4InventoryTags::Slot::Equipments::Weapon)
    {
        TargetSlot = WeaponSlot;
    }
    else if (SlotType == P4InventoryTags::Slot::Equipments::Head)
    {
        TargetSlot = HeadSlot;
    }
    else if (SlotType == P4InventoryTags::Slot::Equipments::Chest)
    {
        TargetSlot = ChestSlot;
    }
    else if (SlotType == P4InventoryTags::Slot::Equipments::Gloves)
    {
        TargetSlot = GlovesSlot;
    }
    else if (SlotType == P4InventoryTags::Slot::Equipments::Legs)
    {
        TargetSlot = LegsSlot;
    }
    else if (SlotType == P4InventoryTags::Slot::Equipments::Shoes)
    {
        TargetSlot = ShoesSlot;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RefreshSlot: 알 수 없는 SlotType[%s]"), *SlotType.ToString());
        return;
    }

    // 해당 슬롯 업데이트
    if (TargetSlot)
    {
        FInventoryItem* Item = EquipInvenComp->GetEquippedItem(SlotType);
        if (Item && Item->ItemData != nullptr)
        {
            TargetSlot->SetItem(*Item);
        }
        else
        {
            TargetSlot->ClearSlot();
        }
    }
}

FReply UP4EquipmentInvenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent&
    InMouseEvent)
{
    // UI가 Hidden 상태면 입력 허용
    if (GetVisibility() == ESlateVisibility::Hidden ||
        GetVisibility() == ESlateVisibility::Collapsed)
    {
        return FReply::Unhandled();
    }

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // DragHeader를 클릭했는지 체크
        if (DragHeader && DragHeader->IsHovered())
        {
            bIsDragging = true;

            // 뷰포트 로컬 좌표로 계산
            if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(EquipPanel->Slot))
            {
                // 현재 패널의 위치 (뷰포트 로컬 좌표)
                FVector2D CurrentPanelPosition = CanvasSlot->GetPosition();

                // 마우스 위치를 뷰포트 로컬 좌표로 변환
                FVector2D MousePosViewport = InMouseEvent.GetScreenSpacePosition();

                if (GEngine && GEngine->GameViewport)
                {
                   /* FVector2D ViewportSize;
                    GEngine->GameViewport->GetViewportSize(ViewportSize);*/

                    // DPI 스케일을 고려한 마우스 위치
                    float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
                    MousePosViewport = MousePosViewport / Scale;
                }

                // 오프셋 = 마우스 위치 - 패널 위치
                DragOffset = MousePosViewport - CurrentPanelPosition;
            }
            return FReply::Handled().CaptureMouse(TakeWidget());
        }

        // 일반 클릭 (DragHeader 아님)
        if (APlayerController* PC = GetOwningPlayer())
        {
            if (AP4PlayerController* P4PC = Cast<AP4PlayerController>(PC))
            {
                P4PC->BringUIToFront(this);
            }
        }

        // DragHeader가 아닌 장비창 내부를 클릭한 경우
        if (EquipPanel && EquipPanel->IsHovered())
        {
            UE_LOG(LogTemp, Log, TEXT("장비창 내부 클릭 - 게임 입력 차단"));
            return FReply::Handled();  // 게임 입력 차단만
        }
    }

    // 우클릭
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        // 장비창 패널 위에서 우클릭하면 차단 (강공격 방지)
        if (EquipPanel && EquipPanel->IsHovered())
        {
            UE_LOG(LogTemp, Log, TEXT("장비창 내부 우클릭 - 게임 입력 차단"));
            return FReply::Handled();  // 우클릭 차단
        }
    }

    return FReply::Unhandled();
}

FReply UP4EquipmentInvenWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(EquipPanel->Slot);

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

        // 화면 경계 제한 추가
        FVector2D PanelSize = EquipPanel->GetCachedGeometry().GetLocalSize();
        FVector2D ViewportSize = FVector2D::ZeroVector;

        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->GetViewportSize(ViewportSize);
            float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
            ViewportSize = ViewportSize / Scale;
        }

        // 경계 제한 (0 ~ 뷰포트 크기 - 패널 크기)
        // 패널 기준이 장비창 패널 기준이라 그 밖에 있는 DragHead는 위로 드래그 하다보면 잘림
        // 그래서 Y값은 Min값을 DragHeader크기만큼 늘려줌
        NewPosition.X = FMath::Clamp(NewPosition.X, 0.0f, FMath::Max(0.0f, ViewportSize.X - PanelSize.X));
        NewPosition.Y = FMath::Clamp(NewPosition.Y, 30.0f, FMath::Max(0.0f, ViewportSize.Y - PanelSize.Y));

        CanvasSlot->SetPosition(NewPosition);

        return FReply::Handled();
    }

    return FReply::Unhandled();
}

FReply UP4EquipmentInvenWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent&
    InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (bIsDragging)
        {
            bIsDragging = false;

            // 드래그 종료 후 최상위로 가져오기
            if (APlayerController* PC = GetOwningPlayer())
            {
                if (AP4PlayerController* P4PC = Cast<AP4PlayerController>(PC))
                {
                    P4PC->BringUIToFront(this);
                }
            }

            return FReply::Handled().ReleaseMouseCapture();
        }

        // 드래그 중이 아니었지만 장비창 내부 클릭이었다면
        if (EquipPanel && EquipPanel->IsHovered())
        {
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
}