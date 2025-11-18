
#pragma once

#include "GameplayTagContainer.h"

//Character.Action
#define P4TAG_CHARACTER_ACTION_ATTACKHITCHECK FGameplayTag::RequestGameplayTag(FName("Character.Action.AttackHitCheck"))

//Character.Interaction
#define P4TAG_CHARACTER_INTERACTION FGameplayTag::RequestGameplayTag(FName("Character.Interaction"))
#define P4TAG_CHARACTER_INTERACTION_ENCHANT FGameplayTag::RequestGameplayTag(FName("Character.Interaction.Enchant"))
#define P4TAG_CHARACTER_INTERACTION_QUEST FGameplayTag::RequestGameplayTag(FName("Character.Interaction.Quest"))

//Character.State
#define P4TAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define P4TAG_CHARACTER_ISDAMAGED   FGameplayTag::RequestGameplayTag(FName("Character.State.IsDamaged"))
#define P4TAG_CHARACTER_ISDEAD      FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define P4TAG_CHARACTER_ISDRAWING   FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawing"))
#define P4TAG_CHARACTER_ISDRAWN     FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"))
#define P4TAG_CHARACTER_ISJUMPING   FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"))
#define P4TAG_CHARACTER_ISSHEATHING FGameplayTag::RequestGameplayTag(FName("Character.State.IsSheathing"))
#define P4TAG_CHARACTER_ISRUNNING FGameplayTag::RequestGameplayTag(FName("Character.State.IsRunning"))

//Monster.Action
#define P4TAG_MONSTER_ACTION_DASHATTACK       FGameplayTag::RequestGameplayTag(FName("Monster.Action.DashAttack"))
#define P4TAG_MONSTER_ACTION_DASHSTART        FGameplayTag::RequestGameplayTag(FName("Monster.Action.DashStart"))
#define P4TAG_MONSTER_ACTION_DOUBLEWATERBOMB  FGameplayTag::RequestGameplayTag(FName("Monster.Action.DoubleWaterBomb"))
#define P4TAG_MONSTER_ACTION_ENERGYBOMB       FGameplayTag::RequestGameplayTag(FName("Monster.Action.EnergyBomb"))
#define P4TAG_MONSTER_ACTION_HOWLING          FGameplayTag::RequestGameplayTag(FName("Monster.Action.Howling"))
#define P4TAG_MONSTER_ACTION_LEFTWINGSTOMP    FGameplayTag::RequestGameplayTag(FName("Monster.Action.LeftWingStomp"))

//Data.Weapon
#define P4TAG_DATA_WEAPON_ATTACK     FGameplayTag::RequestGameplayTag(FName("Data.Weapon.Attack"))
#define P4TAG_DATA_WEAPON_MAXHEALTH  FGameplayTag::RequestGameplayTag(FName("Data.Weapon.MaxHealth"))

//Slot
#define P4TAG_SLOT_CONSUMABLE  FGameplayTag::RequestGameplayTag(FName("Slot.Consumable"))
#define P4TAG_SLOT_EQUIPMENT   FGameplayTag::RequestGameplayTag(FName("Slot.Equipment"))

#define P4TAG_SLOT_EQUIPMENT_ARMOR_CHEST FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Chest"))
#define P4TAG_SLOT_EQUIPMENT_ARMOR_GLOVES FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Gloves"))
#define P4TAG_SLOT_EQUIPMENT_ARMOR_HEAD   FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Head"))
#define P4TAG_SLOT_EQUIPMENT_ARMOR_LEGS   FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Legs"))
#define P4TAG_SLOT_EQUIPMENT_ARMOR_SHOES  FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Armor.Shoes"))

#define P4TAG_SLOT_EQUIPMENT_WEAPON       FGameplayTag::RequestGameplayTag(FName("Slot.Equipment.Weapon"))


//Item.Equipment
#define P4TAG_ITEM_EQUIPMENT_ARMOR       FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor"))
#define P4TAG_ITEM_EQUIPMENT_ARMOR_CHEST FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Chest"))
#define P4TAG_ITEM_EQUIPMENT_ARMOR_GLOVES FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Gloves"))
#define P4TAG_ITEM_EQUIPMENT_ARMOR_HEAD   FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Head"))
#define P4TAG_ITEM_EQUIPMENT_ARMOR_LEGS   FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Legs"))
#define P4TAG_ITEM_EQUIPMENT_ARMOR_SHOES  FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Shoes"))

#define P4TAG_ITEM_EQUIPMENT_SHIELD       FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Shield"))
#define P4TAG_ITEM_EQUIPMENT_WEAPON_SWORD FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon.Sword"))

//Item.Consumable
#define P4TAG_ITEM_CONSUMABLE_POTION_HEALTH FGameplayTag::RequestGameplayTag(FName("Item.Consumable.Potion.Health"))
#define P4TAG_ITEM_CONSUMABLE_POTION_MANA   FGameplayTag::RequestGameplayTag(FName("Item.Consumable.Potion.Mana"))

#define P4TAG_ITEM_CONSUMABLE_UPGRADE            FGameplayTag::RequestGameplayTag(FName("Item.Consumable.Upgrade"))
#define P4TAG_ITEM_CONSUMABLE_UPGRADE_ATTACK      FGameplayTag::RequestGameplayTag(FName("Item.Consumable.Upgrade.Attack"))
#define P4TAG_ITEM_CONSUMABLE_UPGRADE_HEALTH      FGameplayTag::RequestGameplayTag(FName("Item.Consumable.Upgrade.Health"))

