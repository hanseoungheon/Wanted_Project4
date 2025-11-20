// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4BossHpBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Monster/Stat/P4MonsterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"

UP4BossHpBar::UP4BossHpBar()
{

}

void UP4BossHpBar::UpdateHpBar()
{
	if (P4MonsterHpBar == nullptr || MonsterMaxHp < 0.0f)
	{
		return;
	}

	const float Percent = MonsterCurrentHp / MonsterMaxHp;

	P4MonsterHpBar->SetPercent(Percent);
}

void UP4BossHpBar::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	
	//ASC 직접 가져오기.
	ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InOwner);

	if (ASC == nullptr)
	{
		if (InOwner != nullptr && InOwner->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			ASC = Cast<IAbilitySystemInterface>(InOwner)->GetAbilitySystemComponent();
		}
	}

	const UP4MonsterAttributeSet* BossAttr = ASC->GetSet<UP4MonsterAttributeSet>();

	ensureAlways(BossAttr != nullptr);

	MonsterMaxHp = BossAttr->GetMaxHP();
	MonsterCurrentHp = BossAttr->GetCurHP();

	//델리게이트 바인딩.
	ASC->GetGameplayAttributeValueChangeDelegate(UP4MonsterAttributeSet::GetCurHPAttribute())
		.AddUObject(this, &UP4BossHpBar::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UP4MonsterAttributeSet::GetMaxHPAttribute())
		.AddUObject(this, &UP4BossHpBar::OnMaxHealthChanged);

	UpdateHpBar();
}

void UP4BossHpBar::NativeConstruct()
{
	Super::NativeConstruct();

	P4MonsterHpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("P4MonsterHpBar")));

	ensure(P4MonsterHpBar != nullptr);

	SetVisibility(ESlateVisibility::Hidden);
}

void UP4BossHpBar::OnHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	MonsterCurrentHp = ChangedData.NewValue;
	UpdateHpBar();
}

void UP4BossHpBar::OnMaxHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	MonsterMaxHp = ChangedData.NewValue;
	UpdateHpBar();
}