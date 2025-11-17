// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EnchantSlotWidget.h"
#include "UI/P4ItemIconLoader.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Inventory/P4ItemDragDropOperation.h"
UP4EnchantSlotWidget::UP4EnchantSlotWidget()
{
}

void UP4EnchantSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IconLoader == nullptr)
	{
		IconLoader = NewObject<UP4ItemIconLoader>(this);
	}
}

void UP4EnchantSlotWidget::SetItem(const FInventoryItem& InItem)
{
	StoredItem = InItem;

	RefreshItem();
}

void UP4EnchantSlotWidget::ClearItem()
{
	StoredItem = FInventoryItem();
	RefreshItem();
}

void UP4EnchantSlotWidget::RefreshItem()
{
    if (ItemIcon == nullptr)
        return;

    // 아이템이 없으면 빈 이미지
    if (StoredItem.ItemData == nullptr)
    {
        ItemIcon->SetBrushFromTexture(nullptr);
        return;
    }

    // 아이콘 로더 없으면 생성
    if (IconLoader == nullptr)
    {
        IconLoader = NewObject<UP4ItemIconLoader>(this);
    }

    // SoftObjectPtr 아이콘 꺼냄
    TSoftObjectPtr<UTexture2D> IconSoft = StoredItem.ItemData->GetIcon();

    // 비동기로 로딩
    IconLoader->LoadIconAsync(
        IconSoft,
        FOnIconLoadedDelegate::CreateWeakLambda(this, [this](UTexture2D* LoadedTex)
            {
                if (!ItemIcon) return; // 위젯 제거 방지
                ItemIcon->SetBrushFromTexture(LoadedTex);
            })
    );
}

bool UP4EnchantSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UP4ItemDragDropOperation* DragOp = Cast<UP4ItemDragDropOperation>(InOperation);
    if (!DragOp)
    {
        //UE_LOG(LogTemp, Error, TEXT("EnchantSlot: DragOp 캐스팅 실패"));
        return false;
    }

    const FInventoryItem& DraggedItem = DragOp->DraggedItem;
    if (!DraggedItem.ItemData)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnchantSlot: 드래그된 아이템이 없음"));
        return false;
    }

    // 여기서 이 슬롯이 무기 슬롯인지, 강화석 슬롯인지에 따라 필터링 가능
    // ex) BP에서 이 슬롯에 SlotType이나 Tag를 하나 더 두고 필터링해도 됨

    // 일단 아무 조건 없이 받기:
    SetItem(DraggedItem);

    UE_LOG(LogTemp, Log, TEXT("EnchantSlot: 아이템 [%s] 드롭받음"),
        *DraggedItem.ItemData->GetItemName().ToString());

    return true;
}
