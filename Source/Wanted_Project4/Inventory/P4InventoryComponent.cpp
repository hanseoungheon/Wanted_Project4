// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/P4InventoryComponent.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Inventory/P4InventoryTags.h"
#include "Character/P4CharacterBase.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Item/Equipment/P4WeaponComponent.h"
#include "Character/P4CharacterPlayer.h"
#include "Inventory/P4EquipmentInvenComponent.h"

// Sets default values for this component's properties
UP4InventoryComponent::UP4InventoryComponent()
{
    //  배열 초기화
    MaxSlotsPerType.Add(P4InventoryTags::Slot::Equipment, 16);
    MaxSlotsPerType.Add(P4InventoryTags::Slot::Consumable, 16);

    InitiailizeInventoryArrays();
}

bool UP4InventoryComponent::AddItem(UItemDataBase* ItemData, int32 Quantity)
{
    if (!ItemData) return false;

    // 아이템 슬롯 타입 결정
    FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);
    TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

    if (!TargetArray)
    {
        UE_LOG(LogTemp, Error, TEXT("AddItem: TargetArray가 nullptr"));
        return false;
    }

    // 타입별 최대 슬롯 개수
    int32 MaxSlots = MaxSlotsPerType[SlotType];

    // 같은 아이템이 있고 스택 가능하면 먼저 추가
    for (int32 i = 0; i < MaxSlots; ++i)
    {
        if ((*TargetArray)[i].ItemData == ItemData)
        {
            int32 MaxStack = ItemData->GetMaxStackCount();
            int32 CanAdd = FMath::Min(Quantity, MaxStack - (*TargetArray)[i].Quantity);

            if (CanAdd > 0)
            {
                (*TargetArray)[i].Quantity += CanAdd;
                OnInventoryUpdated.Broadcast(SlotType, i);

                if (Quantity - CanAdd > 0)
                {
                    return AddItem(ItemData, Quantity - CanAdd);
                }
                return true;
            }
        }
    }

    // 빈 슬롯 찾기
    for (int32 i = 0; i < MaxSlots; ++i)
    {
        if (!(*TargetArray)[i].ItemData)
        {
            (*TargetArray)[i].ItemData = ItemData;
            (*TargetArray)[i].Quantity = FMath::Min(Quantity, ItemData->GetMaxStackCount());
            (*TargetArray)[i].SlotIndex = i;
            (*TargetArray)[i].SlotType = SlotType;

            OnInventoryUpdated.Broadcast(SlotType, i);

            // 남은 수량이 있으면 재귀 호출
            if (Quantity > (*TargetArray)[i].Quantity)
            {
                return AddItem(ItemData, Quantity - (*TargetArray)[i].Quantity);
            }

            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("인벤토리가 꽉 찼습니다!"));
    return false;
}

bool UP4InventoryComponent::RemoveItem(UItemDataBase* ItemData, int32 Quantity, int32 SlotIndex)
{
    if (!ItemData) return false;

    FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);
    TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

    if (!TargetArray) return false;

    // 특정 슬롯 인덱스가 지정된 경우
    if (SlotIndex >= 0 && TargetArray->IsValidIndex(SlotIndex))
    {
        // 해당 슬롯의 아이템이 맞는지 확인
        if ((*TargetArray)[SlotIndex].ItemData == ItemData)
        {
            (*TargetArray)[SlotIndex].Quantity -= Quantity;

            // 수량이 0 이하면 슬롯 비우기
            if ((*TargetArray)[SlotIndex].Quantity <= 0)
            {
                (*TargetArray)[SlotIndex] = FInventoryItem();
                (*TargetArray)[SlotIndex].SlotType = SlotType;
                (*TargetArray)[SlotIndex].SlotIndex = SlotIndex;
            }

            OnInventoryUpdated.Broadcast(SlotType, SlotIndex);
            UE_LOG(LogTemp, Log, TEXT("RemoveItem: 슬롯[%d]에서 %s %d개 제거"),
                SlotIndex, *ItemData->GetItemName().ToString(), Quantity);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("RemoveItem: 슬롯[%d]의 아이템이 일치하지 않음"), SlotIndex);
            return false;
        }
    }

    // 슬롯 인덱스가 지정되지 않은 경우
    for (int32 i = 0; i < TargetArray->Num(); ++i)
    {
        if ((*TargetArray)[i].ItemData == ItemData)
        {
            (*TargetArray)[i].Quantity -= Quantity;

            // 수량이 0 이하면 슬롯 비우기
            if ((*TargetArray)[i].Quantity <= 0)
            {
                (*TargetArray)[i] = FInventoryItem();
                (*TargetArray)[i].SlotType = SlotType;
                (*TargetArray)[i].SlotIndex = i;
            }

            OnInventoryUpdated.Broadcast(SlotType, i);
            return true;
        }
    }

    return false;
}

bool UP4InventoryComponent::UseItem(UItemDataBase* ItemData, int32 SlotIndex)
{
    if (!ItemData) return false;

    UE_LOG(LogTemp, Log, TEXT("UseItem 함수 호출"));

    // 특정 슬롯이 지정된 경우 해당 슬롯의 아이템 확인
    if (SlotIndex >= 0)
    {
        FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);

        TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

        if (!TargetArray || !TargetArray->IsValidIndex(SlotIndex))
        {
            UE_LOG(LogTemp, Error, TEXT("UseItem: 잘못된 슬롯 인덱스[%d]"), SlotIndex);
            return false;
        }

        if ((*TargetArray)[SlotIndex].ItemData != ItemData)
        {
            UE_LOG(LogTemp, Error, TEXT("UseItem: 슬롯[%d]의 아이템이 일치하지 않음"), SlotIndex);
            return false;
        }

        if ((*TargetArray)[SlotIndex].ItemData != ItemData)
        {
            UE_LOG(LogTemp, Error, TEXT("UseItem: 슬롯[%d]의 아이템이 일치하지 않음"), SlotIndex);
            return false;
        }

        if ((*TargetArray)[SlotIndex].Quantity <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("UseItem: 슬롯[%d]의 아이템 수량이 0개"), SlotIndex);
            return false;
        }
    }
    // 슬롯이 지정되지 않은 경우 전체 개수 확인
    else
    {
        if (GetItemCount(ItemData) <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("아이템이 인벤토리에 없습니다!"));
            return false;
        }
    }

    // 2. Owner의 AbilitySystemComponent 가져오기
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Log, TEXT("GetOwner 실패"));
        return false;
    }

    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
    if (!ASI)
    {
        UE_LOG(LogTemp, Log, TEXT("IAbilitySystemInterface 캐스트 실패"));
        return false;
    }

    UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Log, TEXT("GetAbilitySystemComponent() 실패"));
        return false;
    }

    // 3. GameplayEffect 적용
    TSubclassOf<UGameplayEffect> Effect = ItemData->GetItemEffect();
    if (Effect)
    {
        FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
        EffectContext.AddSourceObject(ItemData);

        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1.0f, EffectContext);
        if (SpecHandle.IsValid())
        {
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            UE_LOG(LogTemp, Log, TEXT("아이템 효과 적용: %s"), *ItemData->GetItemName().ToString());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("SpecHandle.IsValid() 실패"));
        }
    }

    // 4. 소비 아이템이면 수량 감소
    FGameplayTag ConsumableTag = FGameplayTag::RequestGameplayTag(FName("Item.Consumable"));
    if (ItemData->HasTag(ConsumableTag))
    {
        // 특정 슬롯의 아이템 제거
        RemoveItem(ItemData, 1, SlotIndex);
    }

    UE_LOG(LogTemp, Log, TEXT("UseItem 함수 리턴"));
    return true;
}

bool UP4InventoryComponent::EquipItem(UItemDataBase* ItemData, int32 SlotIndex)
{
    if (!ItemData) return false;

    UE_LOG(LogTemp, Log, TEXT("EquipItem 함수 호출"));

    // 특정 슬롯이 지정된 경우 해당 슬롯의 아이템 확인
    if (SlotIndex >= 0)
    {
        FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);
        TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

        if (!TargetArray || !TargetArray->IsValidIndex(SlotIndex))
        {
            UE_LOG(LogTemp, Error, TEXT("EquipItem: 잘못된 슬롯 인덱스[%d]"), SlotIndex);
            return false;
        }

        if ((*TargetArray)[SlotIndex].ItemData != ItemData)
        {
            UE_LOG(LogTemp, Error, TEXT("EquipItem: 슬롯[%d]의 아이템이 일치하지 않음"), SlotIndex);
            return false;
        }

        if ((*TargetArray)[SlotIndex].Quantity <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("EquipItem: 슬롯[%d]의 아이템 수량이 0개"), SlotIndex);
            return false;
        }
    }
    // 슬롯이 지정되지 않은 경우 전체 개수 확인
    else
    {
        if (GetItemCount(ItemData) <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("아이템이 인벤토리에 없습니다!"));
            return false;
        }
    }

    // 장비 태그 확인
    FGameplayTag EquipmentTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment"));
    if (!ItemData->HasTag(EquipmentTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("장비 아이템이 아닙니다!"));
        return false;
    }

    // 이미 착용 중인지 확인
    if (ActiveEquipmentEffects.Contains(ItemData))
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 착용 중인 아이템입니다!"));
        return false;
    }

    

    // AbilitySystemComponent 가져오기
    AActor* Owner = GetOwner();
    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
    if (!ASI) return false;

    UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
    if (!ASC) return false;

    // 패시브 효과 적용
    TSubclassOf<UGameplayEffect> PassiveEffect = ItemData->EquipmentPassiveEffect;
    if (PassiveEffect)
    {
        FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
        EffectContext.AddSourceObject(ItemData);

        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PassiveEffect, 1.0f, EffectContext);
        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

            // 효과 핸들 저장 (나중에 해제할 때 사용)
            ActiveEquipmentEffects.Add(ItemData, EffectHandle);

            UE_LOG(LogTemp, Log, TEXT("장비 착용: %s"), *ItemData->GetItemName().ToString());
        }
    }
    else
    {
        // PassiveEffect가 없어도 ActiveEquipmentEffects에 등록 (더미 핸들)
        ActiveEquipmentEffects.Add(ItemData, FActiveGameplayEffectHandle());
        UE_LOG(LogTemp, Log, TEXT("장비 착용: %s (패시브 효과 없음)"), *ItemData->GetItemName().ToString());
    }

    // // 무기라면 WeaponComponent에 전달 + bIsEquipped 설정
    //FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
    //if (ItemData->HasTag(WeaponTag))
    //{
    //    if (AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(Owner))
    //    {
    //        // WeaponComponent로 무기 장착 (등에 부착)
    //        if (UP4WeaponComponent* WeaponComp = PlayerCharacter->GetWeaponComponent())
    //        {
    //           // WeaponComp->EquipWeapon(ItemData);
    //            bool bEquipSuccess = WeaponComp->EquipWeapon(ItemData);
    //            if (!bEquipSuccess)
    //            {
    //                // 장착 실패 시 인벤토리에서 제거하지 않음
    //                UE_LOG(LogTemp, Warning, TEXT("[EquipItem] 무기 장착 실패 - 인벤토리에서 아이템 제거하지 않음"));
    //                return false;
    //            }
    //        }

    //        //  AnimInstance의 bIsEquipped를 true로 설정
    //        if (USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh())
    //        {
    //            if (UP4PlayerAnimInstance* AnimInst = Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()))
    //            {
    //                AnimInst->bIsEquipped = true;
    //                UE_LOG(LogTemp, Log, TEXT("bIsEquipped = true (등에 무기 장착)"));
    //            }
    //        }
    //    }

    //    // 특정 슬롯의 아이템 제거
    //    RemoveItem(ItemData, 1, SlotIndex);
    //    return true;
    //}

    // 무기라면 WeaponComponent에 전달 + bIsEquipped 설정
    FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
    if (ItemData->HasTag(WeaponTag))
    {
        if (AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(Owner))
        {
            // ⭐ 1. EquipmentInvenComponent에 먼저 등록
            if (UP4EquipmentInvenComponent* EquipInvenComp =
                PlayerCharacter->FindComponentByClass<UP4EquipmentInvenComponent>())
            {
                // 무기 슬롯 태그 가져오기
                FGameplayTag WeaponSlotTag = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Weapon"));

                // 장비창에 아이템 등록
                bool bEquipToSlot = EquipInvenComp->EquipItem(ItemData, WeaponSlotTag);
                if (!bEquipToSlot)
                {
                    UE_LOG(LogTemp, Warning, TEXT("[EquipItem] 장비창 등록 실패"));
                    return false;
                }

                UE_LOG(LogTemp, Log, TEXT("[EquipItem] 장비창에 무기 등록 성공"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[EquipItem] EquipmentInvenComponent를 찾을 수 없음"));
            }

            // ⭐ 2. WeaponComponent로 무기 장착 (등에 부착)
            if (UP4WeaponComponent* WeaponComp = PlayerCharacter->GetWeaponComponent())
            {
                bool bEquipSuccess = WeaponComp->EquipWeapon(ItemData);
                if (!bEquipSuccess)
                {
                    // 장착 실패 시 인벤토리에서 제거하지 않음
                    UE_LOG(LogTemp, Warning, TEXT("[EquipItem] 무기 장착 실패 - 인벤토리에서 아이템 제거하지 않음"));
                    return false;
                }
            }

            // ⭐ 3. AnimInstance의 bIsEquipped를 true로 설정
            if (USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh())
            {
                if (UP4PlayerAnimInstance* AnimInst = Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()))
                {
                    // todo: 자동으로 되는지 바인딩하고 테스트
                    AnimInst->CharacterState.bIsEquipped = true;
                    UE_LOG(LogTemp, Log, TEXT("bIsEquipped = true (등에 무기 장착)"));
                }
            }
        }
    }

    // ⭐ 4. 인벤토리에서 아이템 제거
    RemoveItem(ItemData, 1, SlotIndex);
    return true;
}

bool UP4InventoryComponent::UnequipItem(UItemDataBase* ItemData, int32 SlotIndex)
{
    if (!ItemData) return false;

    // 착용 중인지 확인
    if (!ActiveEquipmentEffects.Contains(ItemData))
    {
        UE_LOG(LogTemp, Warning, TEXT("착용하지 않은 아이템입니다!"));
        return false;
    }

    // AbilitySystemComponent 가져오기
    AActor* Owner = GetOwner();
    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
    if (!ASI) return false;

    UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
    if (!ASC) return false;

    // 효과 제거
    FActiveGameplayEffectHandle EffectHandle = ActiveEquipmentEffects[ItemData];
    // 유효한 핸들인 경우에만 제거
    if (EffectHandle.IsValid())
    {
        ASC->RemoveActiveGameplayEffect(EffectHandle);
        UE_LOG(LogTemp, Log, TEXT("장비 효과 제거: %s"), *ItemData->GetItemName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("장비 효과 없음 (PassiveEffect 없는 아이템): %s"),
            *ItemData->GetItemName().ToString());
    }
    ActiveEquipmentEffects.Remove(ItemData);

    UE_LOG(LogTemp, Log, TEXT("장비 해제: %s"), *ItemData->GetItemName().ToString());

    // 무기라면 WeaponComponent에서 제거 + bIsEquipped 설정
    FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
    if (ItemData->HasTag(WeaponTag))
    {
        if (AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(Owner))
        {
            // WeaponComponent로 무기 해제
            if (UP4WeaponComponent* WeaponComp = PlayerCharacter->GetWeaponComponent())
            {
                WeaponComp->UnequipWeapon();
            }

            // AnimInstance의 bIsEquipped, bIsKatanaOnHand를 false로 설정
            if (USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh())
            {
                if (UP4PlayerAnimInstance* AnimInst = Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()))
                {
                    // todo: 자동 테스트
                    AnimInst->CharacterState.bIsEquipped = false;
                    //AnimInst->bIsKatanaOnHand = false;  // 손에도 없어야 함
                    // 태그 제거
                    if (ASC)
                    {
                        FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
                        ASC->RemoveLooseGameplayTag(DrawnTag);
                    }

                    UE_LOG(LogTemp, Log, TEXT("bIsEquipped = false, bIsKatanaOnHand = false (무기 해제)"));
                }
            }
        }
    }

    return true;
}


void UP4InventoryComponent::SwapSlots(int32 SlotIndexA, int32 SlotIndexB, FGameplayTag SlotType)
{
    TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

    if (!TargetArray)
    {
        UE_LOG(LogTemp, Error, TEXT("SwapSlots: TargetArray가 nullptr"));
        return;
    }

    int32 MaxSlots = MaxSlotsPerType[SlotType];

    // 인덱스 유효성 검사
    if (SlotIndexA < 0 || SlotIndexA >= MaxSlots || SlotIndexB < 0 || SlotIndexB >= MaxSlots)
    {
        UE_LOG(LogTemp, Error, TEXT("SwapSlots: 잘못된 인덱스 (A: %d, B: %d, Max: %d, Type: %s)"),
            SlotIndexA, SlotIndexB, MaxSlots, *SlotType.ToString());
        return;
    }

    // 아이템 교환
    FInventoryItem Temp = (*TargetArray)[SlotIndexA];
    (*TargetArray)[SlotIndexA] = (*TargetArray)[SlotIndexB];
    (*TargetArray)[SlotIndexB] = Temp;

    // SlotIndex 재설정
    (*TargetArray)[SlotIndexA].SlotIndex = SlotIndexA;
    (*TargetArray)[SlotIndexB].SlotIndex = SlotIndexB;

    UE_LOG(LogTemp, Log, TEXT("SwapSlots: 타입[%s] 슬롯[%d] ↔ 슬롯[%d]"),
        *SlotType.ToString(), SlotIndexA, SlotIndexB);

    // UI 갱신 이벤트 브로드캐스트
    OnInventoryUpdated.Broadcast(SlotType, SlotIndexA);
    OnInventoryUpdated.Broadcast(SlotType, SlotIndexB);
}

bool UP4InventoryComponent::HasSpace(UItemDataBase* ItemData) const
{
    if (!ItemData) return false;

    FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);
    const TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

    if (!TargetArray) return false;

    int32 MaxSlots = MaxSlotsPerType[SlotType];

    // 빈 슬롯이 있는지 확인
    for (int32 i = 0; i < MaxSlots; ++i)
    {
        if (!(*TargetArray)[i].ItemData)
        {
            return true;
        }
    }

    return false;
}

int32 UP4InventoryComponent::GetItemCount(UItemDataBase* ItemData) const
{
    if (!ItemData) return 0;

    FGameplayTag SlotType = GetSlotTypeFromItemData(ItemData);
    const TArray<FInventoryItem>* TargetArray = GetInventoryByType(SlotType);

    if (!TargetArray) return 0;

    int32 TotalCount = 0;
    for (const FInventoryItem& Item : *TargetArray)
    {
        if (Item.ItemData == ItemData)
        {
            TotalCount += Item.Quantity;
        }
    }
    return TotalCount;
}

TArray<FInventoryItem>* UP4InventoryComponent::GetInventoryByType(FGameplayTag SlotType)
{
    if (SlotType == P4InventoryTags::Slot::Equipment)
    {
        return &EquipmentItems;
    }
    else if (SlotType == P4InventoryTags::Slot::Consumable)
    {
        return &ConsumableItems;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetInventoryByType: 알 수 없는 SlotType"));
        return nullptr;
    }
}

const TArray<FInventoryItem>* UP4InventoryComponent::GetInventoryByType(FGameplayTag SlotType) const
{
    if (SlotType == P4InventoryTags::Slot::Equipment)
    {
        return &EquipmentItems;
    }
    else if (SlotType == P4InventoryTags::Slot::Consumable)
    {
        return &ConsumableItems;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetInventoryByType: 알 수 없는 SlotType"));
        return nullptr;
    }
}

FGameplayTag UP4InventoryComponent::GetSlotTypeFromItemData(UItemDataBase* ItemData) const
{
    if (!ItemData) return P4InventoryTags::Slot::Equipment;

    if (ItemData->HasTag(P4InventoryTags::Item::Equipment))
    {
        return P4InventoryTags::Slot::Equipment;
    }
    else if (ItemData->HasTag(P4InventoryTags::Item::Consumable))
    {
        return P4InventoryTags::Slot::Consumable;
    }

    return P4InventoryTags::Slot::Equipment; // 기본(임시)
}

void UP4InventoryComponent::InitiailizeInventoryArrays()
{
    // 각 타입별 배열 고정크기로 초기화
    EquipmentItems.SetNum(MaxSlotsPerType[P4InventoryTags::Slot::Equipment]);
    ConsumableItems.SetNum(MaxSlotsPerType[P4InventoryTags::Slot::Consumable]);

    for (int32 i = 0; i < EquipmentItems.Num(); ++i)
    {
        EquipmentItems[i] = FInventoryItem();
        EquipmentItems[i].SlotType = P4InventoryTags::Slot::Equipment;
        EquipmentItems[i].SlotIndex = i;
    }

    for (int32 i = 0; i < ConsumableItems.Num(); ++i)
    {
        ConsumableItems[i] = FInventoryItem();
        ConsumableItems[i].SlotType = P4InventoryTags::Slot::Consumable;
        ConsumableItems[i].SlotIndex = i;
    }
}




