// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Inventory/P4InventoryComponent.h" 
#include "P4EquipmentInvenComponent.generated.h"

// 델리게이트 선언 - 장비가 변경되면 UI에 알림
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentUpdated, FGameplayTag /*EquipSlotType*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANTED_PROJECT4_API UP4EquipmentInvenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP4EquipmentInvenComponent();

    // 장비 장착 함수
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(UItemDataBase* ItemData, FGameplayTag EquipSlotType);

    // 장비 해제 함수
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipItem(FGameplayTag EquipSlotType);

    // 특정 슬롯에 장착된 아이템 가져오기
    //UFUNCTION(BlueprintCallable, Category = "Equipment")
    FInventoryItem* GetEquippedItem(FGameplayTag EquipSlotType);

    // 장비 업데이트 델리게이트
    FOnEquipmentUpdated OnEquipmentUpdated;

protected:
    // 장착된 장비 저장
    // Key: 장비 슬롯 태그 (예: Slot.Equipment.Weapon)
    // Value: 장착된 아이템 정보
    UPROPERTY(EditAnywhere, Category = "Equipment")
    TMap<FGameplayTag, FInventoryItem> EquippedItems;
		
};
