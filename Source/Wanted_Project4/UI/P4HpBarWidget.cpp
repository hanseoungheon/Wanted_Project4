// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"

//#include "Interface/P4CharacterWidgetInterface.h"
UP4HpBarWidget::UP4HpBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	////MaxHp = -1.0f;
	//CurrentHp = 70.0f;
	//MaxHp = 100.0f;
}

void UP4HpBarWidget::UpdateHpBar()
{
	//CurrentHp = NewCurrentHp;
	UE_LOG(LogTemp, Log, TEXT("UpdateHP!"))
	ensureAlways(MaxHp > 0.0f);

	if (P4HpBar != nullptr)
	{
		P4HpBar->SetPercent(CurrentHp / MaxHp);
	}

}

void UP4HpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//위젯 참조를 위한 코드 작성.
	P4HpBar
		= Cast<UProgressBar>(
			GetWidgetFromName(TEXT("P4HpBar"))
		);

	ensureAlways(P4HpBar != nullptr);

	P4ShieldBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("P4ShieldBar")));
	ensure(P4ShieldBar != nullptr);

	//IP4CharacterWidgetInterface* CharacterWidget
	//	= Cast<IP4CharacterWidgetInterface>(OwningActor);

	//if (CharacterWidget != nullptr)
	//{
	//	CharacterWidget->SetupCharacterWidget(this);
	//}
	
}

void UP4HpBarWidget::SetAbilitySystemComponent(AActor* InOwner)
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
	UpdateHpBar();

	MaxShield = Attr->GetMaxShield();
	CurrentShield = Attr->GetShield();
	UpdateShieldBar();

	// 4) 델리게이트 바인딩
	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UP4HpBarWidget::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UP4HpBarWidget::OnMaxHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetShieldAttribute())
		.AddUObject(this, &UP4HpBarWidget::OnShieldChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(UP4PlayerAttributeSet::GetMaxShieldAttribute())
		.AddUObject(this, &UP4HpBarWidget::OnMaxShieldChanged);
}

void UP4HpBarWidget::OnHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	CurrentHp = ChangedData.NewValue;
	UpdateHpBar();
}

void UP4HpBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangedData)
{
	MaxHp = ChangedData.NewValue;
	UpdateHpBar();
}

void UP4HpBarWidget::UpdateShieldBar()
{
	UE_LOG(LogTemp,Log,TEXT("UpdateShield!"))
	if (!P4ShieldBar || MaxShield <= 0.f)
		return;

	const float Percent = CurrentShield / MaxShield;
	P4ShieldBar->SetPercent(Percent);

	// 쉴드 없으면 숨기기
	P4ShieldBar->SetVisibility(MaxShield > 0.f ?
		ESlateVisibility::Visible :
		ESlateVisibility::Collapsed);
}

void UP4HpBarWidget::OnShieldChanged(const FOnAttributeChangeData& Data)
{
	CurrentShield = Data.NewValue;
	UpdateShieldBar();
}

void UP4HpBarWidget::OnMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	MaxShield = Data.NewValue;
	UpdateShieldBar();
}
