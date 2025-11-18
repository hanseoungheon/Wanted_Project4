// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4EnchantSlotWidget.h"
#include "UI/P4ItemIconLoader.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Item/ItemDataBase.h" 
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

    UItemDataBase* ItemData = DraggedItem.ItemData; //아이템 데이터 가저오기.

    //무기종류 태그.
    static const FGameplayTag WeaponTag 
        = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon"));

    //강화재료 종류 태그
    static const FGameplayTag UpgradeTag 
        = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Upgrade"));

    switch (ItemState)
    {
    case EItemState::Weapon:
        if (ItemData->HasTag(WeaponTag) == false)
        {
            UE_LOG(LogTemp, Warning, TEXT("이 슬롯은 무기만 넣을수 있어요!"));
            return false;
        }
        break;

    case EItemState::Upgrade:
        if (ItemData->HasTag(UpgradeTag) == false)
        {
            UE_LOG(LogTemp, Warning, TEXT("이 슬롯은 강화석,강화책만 넣을 수 있어요!"));
            return false;
        }
        break;

    case EItemState::None:
        break;
    }
    //여기까지 왔다는건 무사히 통과했다는것.

    //강화재료가 어떤 종류인지?
    static const FGameplayTag BookTag
        = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Upgrade.Attack"));
    static const FGameplayTag StoneTag
        = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Upgrade.Health"));

    if (ItemData->HasTag(BookTag) == true)
    {
        //만약 책이면 공격력 업그레이드
        UpgradeType 
            = EP4UpgradeType::Attack;
    }
    else if (ItemData->HasTag(StoneTag) == true)
    {
        //만약 돌이면 체력 업그레이드
        UpgradeType 
            = EP4UpgradeType::MaxHealth;
    }


    SetItem(DraggedItem);

    UE_LOG(LogTemp, Log, TEXT("EnchantSlot: 아이템 [%s] 드롭받음"),
        *DraggedItem.ItemData->GetItemName().ToString());

    return true;
}
