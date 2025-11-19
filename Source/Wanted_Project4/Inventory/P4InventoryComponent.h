// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemDataBase.h"
#include "P4InventoryComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, FGameplayTag /*SlotType*/, int32 /*SlotIndex*/);


USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UItemDataBase> ItemData;

	UPROPERTY(EditAnywhere)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int32 SlotIndex = -1;

	// 슬롯 타입 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SlotType;

	// 새 아이템 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsNewItem = false;

	//EInventorySlotType SlotType = EInventorySlotType::Equipment;

	FInventoryItem()
		: ItemData(nullptr), Quantity(0), SlotIndex(-1), SlotType(FGameplayTag())
	{
	}

	// 비교 연산자 추가
	bool operator==(const FInventoryItem& Other) const
	{
		return ItemData == Other.ItemData && SlotIndex == Other.SlotIndex;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANTED_PROJECT4_API UP4InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP4InventoryComponent();


public:
	// 아이템 추가 함수
	bool AddItem(UItemDataBase* ItemData, int32 Quantity);

	// 아이템 제거 함수
	bool RemoveItem(UItemDataBase* ItemData, int32 Quantity, int32 SlotIndex = -1);

	// 아이템 사용 함수 추가
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UItemDataBase* ItemData, int32 SlotIndex = -1);

	// 장비 착용/해제 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool EquipItem(UItemDataBase* ItemData, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UnequipItem(UItemDataBase* ItemData, int32 SlotIndex = -1);

	// 슬롯 교환
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapSlots(int32 SlotIndexA, int32 SlotIndexB, FGameplayTag SlotType);

	// 인벤토리 공간 확인 함수 추가
	bool HasSpace(UItemDataBase* ItemData) const;
	int32 GetItemCount(UItemDataBase* ItemData) const;

	// 타입별 인벤토리 데이터 가져오기
	TArray<FInventoryItem>* GetInventoryByType(FGameplayTag SlotType);
	const TArray<FInventoryItem>* GetInventoryByType(FGameplayTag SlotType) const;

	// 아이템 태그로 슬롯 타입 결정
	FGameplayTag GetSlotTypeFromItemData(UItemDataBase* ItemData) const;

	// 인벤토리가 변경되었을 때 알림을 보내는 델리게이트
	FOnInventoryUpdated OnInventoryUpdated;

public:
	// 최대 슬롯 개수 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FGameplayTag, int32> MaxSlotsPerType;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FInventoryItem> EquipmentItems;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FInventoryItem> ConsumableItems;

	// 착용 중인 장비 효과 핸들 저장
	TMap<UItemDataBase*, FActiveGameplayEffectHandle> ActiveEquipmentEffects;

private:
	// 초기화
	void InitiailizeInventoryArrays();
};
