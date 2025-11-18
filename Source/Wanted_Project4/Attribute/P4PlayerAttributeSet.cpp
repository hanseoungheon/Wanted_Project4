// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/P4PlayerAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/P4GameplayTag.h"

UP4PlayerAttributeSet::UP4PlayerAttributeSet() :
	AttackRange(250.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	DamageAmount(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UP4PlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAmountAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UP4PlayerAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	//if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	//{
	//	if (Data.EvaluatedData.Magnitude > 0.0f)
	//	{
	//		if (Data.Target.HasMatchingGameplayTag(ABTAG_CHARACTER_INVINSIBLE))
	//		{
	//			Data.EvaluatedData.Magnitude = 0.0f;
	//			return false;
	//		}
	//	}
	//}

	return true;
}

void UP4PlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/*float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	*/


	//작성 한승헌 - 2025-11-10
	//작성 이선우 - 2025-11-14 DamageAmount로 수정
	const float MinHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAmountAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth() - GetDamageAmount(), MinHealth, GetMaxHealth());

		SetHealth(NewHealth);

		SetDamageAmount(0.0f);
	}
	//==================================================


	if ((GetHealth() <= 0.0f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("HP 00000000"));
		Data.Target.AddLooseGameplayTag(P4TAG_CHARACTER_ISDEAD);
		OnHpZero.Broadcast();
	}

	// bOutOfHealth 제거
	//bOutOfHealth = (GetHealth() <= 0.0f);
	//UE_LOG(LogTemp, Warning, TEXT("%d"), bOutOfHealth);
}