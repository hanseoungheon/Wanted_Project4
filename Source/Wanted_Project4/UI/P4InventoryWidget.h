// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "GameplayTagContainer.h"
#include "Inventory/P4InventoryComponent.h"
#include "P4InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4InventoryWidget : public UP4CustomWidget
{
	GENERATED_BODY()

public:
	UP4InventoryWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	// 소멸자에서 델리게이트 해제
	virtual void BeginDestroy() override;

	// 인벤토리 컴포넌트랑 바인드
	void BindInventory(class UP4InventoryComponent* InInventoryComp);

	// UI 업데이트
	void RefreshUI();

	// 특정 슬롯만 업데이트
	void RefreshSlot(FGameplayTag SlotType, int32 SlotIndex);

	// 마우스 클릭 감지(인벤토리 클릭 시 공격모션 방지하기 위해)
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	// 마우스 움직임 감지
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// 마우스 버튼 뗌 감지
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	//// 마우스가 인벤토리 영역 위에 있는지 체크
	//bool IsMouseOverInventory() const;

public:
	// 인벤토리 컴포넌트로 인벤토리 데이터 참조
	UPROPERTY()
	TObjectPtr<class UP4InventoryComponent> InventoryComp;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UP4Slot>> EquipmentSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UP4Slot>> ConsumableSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> EquipmentTypeSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ConsumableTypeSlot;

private:
	bool bIsDragging;
	
	// 오프셋
	FVector2D DragOffset;

	// 인벤토리 영역 Canvas Panel (모든 인벤토리 UI를 담고 있는 패널)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> InventoryPanel;

	// 인벤토리 드래그 하기 위한 헤더보드
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> DragHeader;
};

