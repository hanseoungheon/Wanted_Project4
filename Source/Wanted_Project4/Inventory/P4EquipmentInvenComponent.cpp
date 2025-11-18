// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/P4EquipmentInvenComponent.h"
#include "Inventory/P4InventoryTags.h"

// Sets default values for this component's properties
UP4EquipmentInvenComponent::UP4EquipmentInvenComponent()
{
    // EquippedItems 초기화
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Weapon, FInventoryItem());
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Head, FInventoryItem());
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Chest, FInventoryItem());
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Gloves, FInventoryItem());
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Legs, FInventoryItem());
    EquippedItems.Add(P4InventoryTags::Slot::Equipments::Shoes, FInventoryItem());
}

bool UP4EquipmentInvenComponent::EquipItem(UItemDataBase* ItemData, FGameplayTag EquipSlotType)
{
    // 유효성 검사
    if (!ItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("[EquipmentInvenComp] ItemData가 nullptr"));
        return false;
    }

    // 장비 슬롯 태그가 유효한지 확인
    if (!EquippedItems.Contains(EquipSlotType))
    {
        UE_LOG(LogTemp, Error, TEXT("[EquipmentInvenComp] 유효하지 않은 장비 슬롯 태그: %s"),
            *EquipSlotType.ToString());
        return false;
    }

    // 이미 해당 슬롯에 아이템이 장착되어 있는지 확인
    FInventoryItem& CurrentEquipped = EquippedItems[EquipSlotType];
    if (CurrentEquipped.ItemData != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[EquipmentInvenComp] 슬롯[%s]에 이미 아이템이 장착되어 있음: %s"),
            *EquipSlotType.ToString(),
            *CurrentEquipped.ItemData->GetItemName().ToString());

        // 기존 아이템을 먼저 해제해야 함
        // 또는 자동으로 교체하려면 여기서 UnequipItem 호출
        return false;
    }

    // 아이템 장착
    FInventoryItem NewEquipItem;
    NewEquipItem.ItemData = ItemData;
    NewEquipItem.Quantity = 1;  // 장비는 보통 수량 1
    NewEquipItem.SlotType = EquipSlotType;

    EquippedItems[EquipSlotType] = NewEquipItem;

    UE_LOG(LogTemp, Log, TEXT("[EquipmentInvenComp] 장비 장착 성공: %s -> 슬롯[%s]"),
        *ItemData->GetItemName().ToString(),
        *EquipSlotType.ToString());

    // UI 업데이트를 위한 델리게이트 브로드캐스트
    OnEquipmentUpdated.Broadcast(EquipSlotType);

    return true;
}

bool UP4EquipmentInvenComponent::UnequipItem(FGameplayTag EquipSlotType)
{
    // 장비 슬롯 태그가 유효한지 확인
    if (!EquippedItems.Contains(EquipSlotType))
    {
        UE_LOG(LogTemp, Error, TEXT("[EquipmentInvenComp] 유효하지 않은 장비 슬롯 태그: %s"),
            *EquipSlotType.ToString());
        return false;
    }

    // 해당 슬롯에 아이템이 장착되어 있는지 확인
    FInventoryItem& CurrentEquipped = EquippedItems[EquipSlotType];
    if (CurrentEquipped.ItemData == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[EquipmentInvenComp] 슬롯[%s]에 장착된 아이템이 없음"),
            *EquipSlotType.ToString());
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("[EquipmentInvenComp] 장비 해제: %s (슬롯[%s])"),
        *CurrentEquipped.ItemData->GetItemName().ToString(),
        *EquipSlotType.ToString());

    // 슬롯을 빈 상태로 변경
    EquippedItems[EquipSlotType] = FInventoryItem();

    // UI 업데이트를 위한 델리게이트 브로드캐스트
    OnEquipmentUpdated.Broadcast(EquipSlotType);

    return true;
}

FInventoryItem* UP4EquipmentInvenComponent::GetEquippedItem(FGameplayTag EquipSlotType)
{
    // 유효한 슬롯인지 확인
    if (!EquippedItems.Contains(EquipSlotType))
    {
        UE_LOG(LogTemp, Error, TEXT("[EquipmentInvenComp] 유효하지 않은 장비 슬롯 태그: %s"),
            *EquipSlotType.ToString());
        return nullptr;
    }

    // 맵에서 해당 슬롯의 아이템 포인터 반환
    return &EquippedItems[EquipSlotType];
}
