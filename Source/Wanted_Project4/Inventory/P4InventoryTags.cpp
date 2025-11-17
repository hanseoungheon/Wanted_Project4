#include "Inventory/P4InventoryTags.h"
#include "GameplayTagsManager.h"

namespace P4InventoryTags
{
    namespace Item
    {
        const FGameplayTag Equipment = FGameplayTag::RequestGameplayTag(FName("Item.Equipment"));
        const FGameplayTag Consumable = FGameplayTag::RequestGameplayTag(FName("Item.Consumable"));
        const FGameplayTag Quest = FGameplayTag::RequestGameplayTag(FName("Item.Quest"));
        const FGameplayTag Material = FGameplayTag::RequestGameplayTag(FName("Item.Material"));
    }

    namespace Slot
    {
        const FGameplayTag Equipment = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment"));
        const FGameplayTag Consumable = FGameplayTag::RequestGameplayTag(FName("Slot.Consumable"));
    }
}