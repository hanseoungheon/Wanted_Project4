#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 인벤토리 시스템에서 사용하는 GameplayTag 정의
 */
namespace P4InventoryTags
{
    // 아이템 타입 태그
    namespace Item
    {
        WANTED_PROJECT4_API extern const FGameplayTag Equipment;
        WANTED_PROJECT4_API extern const FGameplayTag Consumable;

        // 추가 확장 가능
        //WANTED_PROJECT4_API extern const FGameplayTag Quest;
        //WANTED_PROJECT4_API extern const FGameplayTag Material;
    }

    namespace Slot
    {
        WANTED_PROJECT4_API extern const FGameplayTag Equipment;
        WANTED_PROJECT4_API extern const FGameplayTag Consumable;

        namespace Equipments
        {
            WANTED_PROJECT4_API extern const FGameplayTag Weapon;
            namespace Armor
            {
                WANTED_PROJECT4_API extern const FGameplayTag Head;
                WANTED_PROJECT4_API extern const FGameplayTag Chest;
                WANTED_PROJECT4_API extern const FGameplayTag Gloves;
                WANTED_PROJECT4_API extern const FGameplayTag Legs;
                WANTED_PROJECT4_API extern const FGameplayTag Shoes;
            }
        }
    }
}