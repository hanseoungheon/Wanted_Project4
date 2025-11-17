// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/GE/P4GE_EnchantWeapon.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"




UP4GE_EnchantWeapon::UP4GE_EnchantWeapon()
{
    // 옵션: 여기서 DurationPolicy 바꾸는 정도만 할 수 있음
    DurationPolicy = EGameplayEffectDurationType::Infinite;
}
