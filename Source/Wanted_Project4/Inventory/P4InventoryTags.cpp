#include "Inventory/P4InventoryTags.h"
#include "GameplayTagsManager.h"

namespace P4InventoryTags
{
    namespace Item
    {
        const FGameplayTag Equipment = FGameplayTag::RequestGameplayTag(FName("Item.Equipment"));
        const FGameplayTag Consumable = FGameplayTag::RequestGameplayTag(FName("Item.Consumable"));
        /*const FGameplayTag Quest = FGameplayTag::RequestGameplayTag(FName("Item.Quest"));
        const FGameplayTag Material = FGameplayTag::RequestGameplayTag(FName("Item.Material"));*/
    }

    namespace Slot
    {
        const FGameplayTag Equipment = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment"));
        const FGameplayTag Consumable = FGameplayTag::RequestGameplayTag(FName("Slot.Consumable"));

        namespace Equipments
        {
            const FGameplayTag Weapon = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Weapon"));
            namespace Armor
            {
                const FGameplayTag Head = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Head"));
                const FGameplayTag Chest = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Chest"));
                const FGameplayTag Gloves = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Gloves"));
                const FGameplayTag Legs = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Legs"));
                const FGameplayTag Shoes = FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Shoes"));
            }
        }
    }
}