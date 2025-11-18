// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/P4Slot.h"
#include "Item/ItemDataBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "UI/P4ItemIconLoader.h"
#include "Inventory/P4InventoryComponent.h"
#include "Inventory/P4InventoryTags.h"
#include "Test/P4TestPlayerController.h"

#include "Blueprint/DragDropOperation.h"
#include "Layout/Geometry.h"
#include "Input/Events.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "P4ItemDragDropOperation.h"
#include "Components/SizeBox.h"

#include "Inventory/P4EquipmentInvenComponent.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h" 
#include "AbilitySystemGlobals.h"

UP4Slot::UP4Slot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UP4Slot::NativeConstruct()
{
	Super::NativeConstruct();

	IMG_Item = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Item")));
	ensureAlways(IMG_Item);

	TXT_Quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Quantity")));
	ensureAlways(TXT_Quantity);
}

FReply UP4Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 우클릭 입력
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		// 해당 슬롯에 아이템 정보가 존재하는지 체크
		if (CurrentItem.ItemData)
		{
			// ⭐ ItemData를 미리 복사 (델리게이트로 CurrentItem이 초기화될 수 있기 때문)
			UItemDataBase* ItemDataCache = CurrentItem.ItemData;
			FGameplayTag SlotTypeCache = SlotType;
			int32 SlotIndexCache = SlotIndex;

			if (APlayerController* PC = GetOwningPlayer())
			{
				if (APawn* Pawn = PC->GetPawn())
				{
					// 인벤토리 슬롯인지 장비창 슬롯인지 구분
					if (bIsEquipmentSlot)
					{
						// 장비창 슬롯 우클릭 → 장비 해제
						UE_LOG(LogTemp, Log, TEXT("[P4Slot] 장비창 슬롯 우클릭 - 해제 시도: %s"),
							*ItemDataCache->GetItemName().ToString());

						// ⭐ 무기인 경우, 발도 상태(Character.State.IsDrawn)면 해제 불가
						FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
						if (ItemDataCache->HasTag(WeaponTag))
						{
							// ASC 가져오기
							if (UAbilitySystemComponent* AbilitySystemComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn))
							{
								FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
								if (AbilitySystemComp->HasMatchingGameplayTag(DrawnTag))
								{
									UE_LOG(LogTemp, Warning, TEXT("[P4Slot] 발도 상태에서는 장비 해제 불가! 먼저 납도하세요."));
									return FReply::Handled();
								}
							}
						}

						if (UP4EquipmentInvenComponent* EquipInvenComp =
							Pawn->FindComponentByClass<UP4EquipmentInvenComponent>())
						{
							if (UP4InventoryComponent* InvComp = Pawn->FindComponentByClass<UP4InventoryComponent>())
							{
								// 인벤토리에 빈 공간이 있는지 확인
								if (!InvComp->HasSpace(ItemDataCache))
								{
									UE_LOG(LogTemp, Warning, TEXT("[P4Slot] 인벤토리 공간 부족!"));
									return FReply::Handled();
								}

								// 무기인 경우 WeaponComponent + InventoryComponent의 UnequipItem 먼저 호출
								//FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
								if (ItemDataCache->HasTag(WeaponTag))
								{
									UE_LOG(LogTemp, Log, TEXT("[P4Slot] 무기 해제 시작"));

									// InventoryComponent의 UnequipItem 호출 (WeaponComponent 해제 + GE 제거)
									bool bUnequipFromInv = InvComp->UnequipItem(ItemDataCache, -1);
									if (!bUnequipFromInv)
									{
										UE_LOG(LogTemp, Error, TEXT("[P4Slot] InventoryComponent 무기 해제 실패"));
										return FReply::Handled();
									}
								}

								// EquipmentInvenComponent에서 해제
								bool bUnequipSuccess = EquipInvenComp->UnequipItem(SlotTypeCache);
								if (!bUnequipSuccess)
								{
									UE_LOG(LogTemp, Error, TEXT("[P4Slot] EquipmentInvenComponent 해제 실패"));
									return FReply::Handled();
								}

								// 인벤토리에 아이템 추가
								bool bAddSuccess = InvComp->AddItem(ItemDataCache, 1);
								if (bAddSuccess)
								{
									UE_LOG(LogTemp, Log, TEXT("[P4Slot] 장비 해제 성공 - 인벤토리에 추가됨"));
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT("[P4Slot] 인벤토리 추가 실패"));
								}
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("[P4Slot] InventoryComponent 찾을 수 없음"));
							}
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("[P4Slot] EquipmentInvenComponent 찾을 수 없음"));
						}
					}
					else
					{
						// 인벤토리 슬롯 우클릭 → 장비 장착 또는 아이템 사용
						if (UP4InventoryComponent* InvComp = Pawn->FindComponentByClass<UP4InventoryComponent>())
						{
							if (ItemDataCache->HasTag(P4InventoryTags::Item::Equipment))
							{
								InvComp->EquipItem(ItemDataCache, SlotIndexCache);
							}
							else if (ItemDataCache->HasTag(P4InventoryTags::Item::Consumable))
							{
								InvComp->UseItem(ItemDataCache, SlotIndexCache);
							}
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("인벤토리 컴포넌트를 찾지 못함"));
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("폰을 찾지 못함"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("플레이어 컨트롤러를 찾지 못함"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("아이템 없음"));
		}

		// 우클릭은 항상 Handled 반환 (게임 입력 차단)
		return FReply::Handled();
	}

	// 좌클릭 입력
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// 해당 슬롯에 아이템 정보가 존재하는지 체크
		if (CurrentItem.ItemData)
		{
			// 유효한 아이템이 존재하면 드래그 이벤트를 감지하도록 UWidgetBlueprintLibrary::DetectDragIfPressed 함수를 호출합니다.
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return Reply.NativeReply;
		}
		else
		{
			// 아이템이 없어도 슬롯 클릭은 Handled (공격 차단)
			return FReply::Handled();
		}
	}

	// 다른 버튼이면 Handled 반환 (안전장치)
	return FReply::Handled();
}

void UP4Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 아이템 유효성 확인
	if (!CurrentItem.ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentItem.ItemData가 nullptr 입니다."));
		return;
	}

	// 드래그 비주얼 클래스 확인
	if (!DragWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DragVisualClass가 에디터에서 설정되지 않았습니다."));
		return;
	}

	// 월드 유효성 체크
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld()가 nullptr 입니다."));
		return;
	}

	// 최상위 CanvasPanel 생성
	UCanvasPanel* Canvas = NewObject<UCanvasPanel>(this);

	// SizeBox를 만들어서 강제 크기 지정
	USizeBox* SizeBox = NewObject<USizeBox>(Canvas);
	SizeBox->SetWidthOverride(64.f);
	SizeBox->SetHeightOverride(64.f);

	// 드래그 오퍼레이션 생성
	UP4ItemDragDropOperation* DragOp = NewObject<UP4ItemDragDropOperation>();
	DragOp->DraggedItem = CurrentItem; 
	DragOp->FromSlot = this;           
	DragOp->Payload = this;
	// 강제크기로 만든 이미지 생성
	UImage* ItemImage = NewObject<UImage>(SizeBox);
	if (ItemImage && CurrentItem.ItemData->GetIcon().IsValid())
	{
		// 현재 아이템 이미지 동기 애셋 로딩
		if (UTexture2D* Icon = CurrentItem.ItemData->GetIcon().LoadSynchronous())
		{
			ItemImage->SetBrushFromTexture(Icon);
			//ItemImage->SetDesiredSizeOverride(FVector2D(1000.f, 1000.f)); // 원하는 크기로 조절
			//ItemImage->SetOpacity(1.0f);
		}

		// 계층 구성
		SizeBox->AddChild(ItemImage);
		Canvas->AddChild(SizeBox);

		
		DragOp->DefaultDragVisual = SizeBox; // 바로 설정
	}

	OutOperation = DragOp;

}

bool UP4Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// 드래그 오퍼레이션 캐스팅
	UP4ItemDragDropOperation* DropOp = Cast<UP4ItemDragDropOperation>(InOperation);
	if (!DropOp)
	{
		UE_LOG(LogTemp, Error, TEXT("DragDropOperation 캐스팅 실패"));
		return false;
	}

	if (!DropOp->FromSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("FromSlot이 nullptr입니다."));
		return false;
	}

	// 같은 슬롯에 드롭한 경우 - 아무것도 하지 않음
	if (DropOp->FromSlot == this)
	{
		return true;
	}

	// 다른 타입의 슬롯으로 드래그 방지
	if (DropOp->FromSlot->SlotType != this->SlotType)
	{
		return false;
	}

	// 인벤토리 컴포넌트에서만 교환 (UI는 RefreshUI로 자동 갱신)
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			if (UP4InventoryComponent* InvComp = Pawn->FindComponentByClass<UP4InventoryComponent>())
			{
				// 인벤토리 컴포넌트에서 교환 → OnInventoryUpdated 브로드캐스트 → RefreshUI 자동 호출
				InvComp->SwapSlots(DropOp->FromSlot->SlotIndex, SlotIndex, SlotType);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("인벤토리 컴포넌트를 찾지 못함"));
			}
		}
	}

	return false;
}

void UP4Slot::SetItem(const FInventoryItem& InItemData)
{
	CurrentItem = InItemData;

	UpdateSlotUI();
}

void UP4Slot::UpdateSlotUI()
{
	// 아이템이 없으면 빈 슬롯 표시
	if (!CurrentItem.ItemData)
	{
		IMG_Item->SetBrushFromTexture(DefaultTexture);
		TXT_Quantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// IconLoader 생성
	if (!IconLoader)
	{
		IconLoader = NewObject<UP4ItemIconLoader>(this);
	}

	// 아이콘 비동기 로딩
	TSoftObjectPtr<UTexture2D> Icon = CurrentItem.ItemData->GetIcon();
	IconLoader->LoadIconAsync(Icon, FOnIconLoadedDelegate::CreateWeakLambda(this, [this](UTexture2D* LoadedIcon)
		{
			if (LoadedIcon)
			{
				IMG_Item->SetBrushFromTexture(LoadedIcon);
			}
			else
			{
				IMG_Item->SetBrushFromTexture(DefaultTexture);
			}
		}));

	// 수량 표시
	if (CurrentItem.Quantity > 1)
	{
		TXT_Quantity->SetText(FText::AsNumber(CurrentItem.Quantity));
		TXT_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TXT_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UP4Slot::ClearSlot()
{
	CurrentItem = FInventoryItem();
	IMG_Item->SetBrushFromTexture(DefaultTexture);

	// 수량 텍스트도 숨기기
	if (TXT_Quantity)
	{
		TXT_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}


