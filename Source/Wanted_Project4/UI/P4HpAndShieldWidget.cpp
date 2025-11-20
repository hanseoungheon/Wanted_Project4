// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4HpAndShieldWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/SlateWrapperTypes.h"

#include "Attribute/P4PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"

UP4HpAndShieldWidget::UP4HpAndShieldWidget()
{
}

void UP4HpAndShieldWidget::UpdateStat()
{
	const float ClampedMaxHp = FMath::Max(MaxHp, 1.0f);

	float Hp = FMath::Clamp(CurrentHp, 0.0f, ClampedMaxHp);
	float Shield = FMath::Max(CurrentShield, 0.0f);

	float TotalUsed = FMath::Min(Hp + Shield, ClampedMaxHp);
	float Empty = ClampedMaxHp - TotalUsed;

	if (!HpSlot || !ShieldSlot || !EmptySlot)
		return;

	FSlateChildSize HpSize(ESlateSizeRule::Fill);
	HpSize.Value = Hp;
	HpSlot->SetSize(HpSize);

	FSlateChildSize ShieldSize(ESlateSizeRule::Fill);
	ShieldSize.Value = Shield;
	ShieldSlot->SetSize(ShieldSize);

	FSlateChildSize EmptySize(ESlateSizeRule::Fill);
	EmptySize.Value = Empty;
	EmptySlot->SetSize(EmptySize);

	ImgShieldFill->SetVisibility(Shield > 0.f ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UP4HpAndShieldWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	// 1) ASC 직접 확보
	ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InOwner);
	if (!ASC)
	{
		if (InOwner && InOwner->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			ASC = Cast<IAbilitySystemInterface>(InOwner)->GetAbilitySystemComponent();
		}
	}
	if (!ASC) { UE_LOG(LogTemp, Error, TEXT("[HpBar] ASC not found")); return; }

	// 2) AttributeSet 확보
	const UP4PlayerAttributeSet* Attr = ASC->GetSet<UP4PlayerAttributeSet>();
	if (!Attr) { UE_LOG(LogTemp, Error, TEXT("[HpBar] Attr not found")); return; }

	// 3) 초기값 즉시 반영 (★ 중요: 처음에 0%로 보이는 문제 해결)
	MaxHp = Attr->GetMaxHealth();
	CurrentHp = Attr->GetHealth();

	MaxShield = Attr->GetMaxShield();
	CurrentShield = Attr->GetShield();

	UpdateStat();

	// 4) 델리게이트 바인딩
	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UP4HpAndShieldWidget::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UP4HpAndShieldWidget::OnMaxHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetShieldAttribute())
		.AddUObject(this, &UP4HpAndShieldWidget::OnShieldChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetMaxShieldAttribute())
		.AddUObject(this, &UP4HpAndShieldWidget::OnMaxShieldChanged);
}

void UP4HpAndShieldWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ImgHpFill = Cast<UImage>(GetWidgetFromName(TEXT("ImgHpFill")));
	ImgShieldFill = Cast<UImage>(GetWidgetFromName(TEXT("ImgShieldFill")));
	ImgEmptyFill = Cast<UImage>(GetWidgetFromName(TEXT("ImgEmptyFill")));
	FillBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("FillBox")));

	ensure(ImgHpFill && ImgShieldFill && ImgEmptyFill && FillBox);

	HpSlot = Cast<UHorizontalBoxSlot>(ImgHpFill->Slot);
	ShieldSlot = Cast<UHorizontalBoxSlot>(ImgShieldFill->Slot);
	EmptySlot = Cast<UHorizontalBoxSlot>(ImgEmptyFill->Slot);

	ensure(HpSlot && ShieldSlot && EmptySlot);
}

void UP4HpAndShieldWidget::OnHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	CurrentHp = ChangedData.NewValue;
	UpdateStat();
}

void UP4HpAndShieldWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	MaxHp = ChangedData.NewValue;
	UpdateStat();
}

void UP4HpAndShieldWidget::OnShieldChanged(const FOnAttributeChangeData& Data)
{
	CurrentShield = Data.NewValue;
	UpdateStat();
}

void UP4HpAndShieldWidget::OnMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	MaxShield = Data.NewValue;  // 필요하면 MaxShield도 써서 따로 제한을 줄 수 있음 
	UpdateStat();
}