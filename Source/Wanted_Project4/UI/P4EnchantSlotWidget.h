// P4EnchantSlotWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/P4CustomWidget.h"
#include "Inventory/P4InventoryComponent.h"
#include "Game/P4UpgradeType.h"
#include "P4EnchantSlotWidget.generated.h"


UENUM()
enum class EItemState
{
    Upgrade UMETA(DisplayName = "Upgrade"),
    Weapon  UMETA(DisplayName = "Weapon"),
    None    UMETA(DisplayName = "None")
};

UCLASS()
class WANTED_PROJECT4_API UP4EnchantSlotWidget : public UP4CustomWidget
{
    GENERATED_BODY()

public:
    UP4EnchantSlotWidget();

    virtual void NativeConstruct() override;

    // 인벤토리에서 드래그된 아이템 넣기
    void SetItem(const FInventoryItem& InItem);

    // 슬롯 비우기
    void ClearItem();

    // 내부 아이콘/수량 갱신
    void RefreshItem();

    // 헷갈리니까 이건 지우는 게 좋음
    void CleatItem() { ClearItem(); }

    // 아이템이 들어있는지?
    UFUNCTION(BlueprintPure, Category = Enchant)
    bool HasItem() const { return StoredItem.ItemData != nullptr; }

    // 저장된 FInventoryItem 가져오기
    const FInventoryItem& GetStoredItem() const { return StoredItem; }

    // 드롭 받기
    virtual bool NativeOnDrop(
        const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;



public:
    // 인벤토리에서 가져온 아이템
    FInventoryItem StoredItem;

    //블랭크 이미지.(기본 비어있는 이미지)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UTexture2D> OriginalTexture;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> ItemIcon;

    UPROPERTY()
    TObjectPtr<class UP4ItemIconLoader> IconLoader;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enchant)
    EItemState ItemState = EItemState::None;


    EP4UpgradeType UpgradeType = EP4UpgradeType::None;
};