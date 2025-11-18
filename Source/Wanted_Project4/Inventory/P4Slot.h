// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GameplayTagContainer.h"
#include "P4InventoryComponent.h"
#include "P4Slot.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4Slot : public UP4CustomWidget
{
	GENERATED_BODY()
	
public:
	UP4Slot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// 마우스 클릭 감지
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	// 마우스 드래그 감지
	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	
	// 마우스 드랍 감지
	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	// 아이템 인벤토리에 세팅
	void SetItem(const FInventoryItem& InItemData);

	// UI만 업데이트 (CurrentItem은 그대로)
	void UpdateSlotUI();

	void ClearSlot();

	// 현재 아이템 정보 가져오기
	FORCEINLINE const FInventoryItem& GetCurrentItem() const { return CurrentItem; }
public:
	/*UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UItemDataBase> ItemData;*/

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UImage> IMG_Item;

	// 기본 빈 슬롯 텍스처
	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTextBlock> TXT_Quantity;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;

	// 슬롯 타입 (에디터에서 설정)
	UPROPERTY(EditAnywhere, Category = "Slot")
	FGameplayTag SlotType;// = EInventorySlotType::Equipment;

	// 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	FGameplayTagContainer ItemTags;

	// 이 슬롯이 어느 위젯에 속해있는지 구분 (인벤토리 or 장비창)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	bool bIsEquipmentSlot = false;  // true면 장비창 슬롯, false면 인벤토리 슬롯
protected:
	// 슬롯이 현재 가진 아이템 정보
	FInventoryItem CurrentItem;

	UPROPERTY()
	TObjectPtr<class UP4ItemIconLoader> IconLoader;

	// 드래그 할때 사용될 아이템 이미지 위젯
	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UP4Slot> DragWidgetClass;
};
