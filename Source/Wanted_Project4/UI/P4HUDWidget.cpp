// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4HUDWidget.h"
#include "P4HpBarWidget.h"
#include "P4BossHpBar.h"

#include "P4QuestInformationWidget.h"
#include "P4MiniMapWidget.h"
#include "Interface/P4CharacterHUDInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Monster/P4BossMonsterBase.h"

UP4HUDWidget::UP4HUDWidget(const FObjectInitializer& ObjectInitalizer)
	:Super(ObjectInitalizer)
{

}

//작성 한승헌 GAS사용시 이건 필요가 없음
//void UP4HUDWidget::UpdateHpBar(float NewCurrentHp)
//{
//	HpBar->UpdateHpBar(NewCurrentHp);
//}

void UP4HUDWidget::UpdataMiniMap(FVector PlayerLocation)
{

}

void UP4HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//데이터 연동.
	//HpBar =  Cast<UP4HpBarWidget>(
	//	GetWidgetFromName(TEXT("widgethpbar"))
	//);
	//ensureAlways(HpBar != nullptr);

	QuestTextBlock = Cast<UP4QuestInformationWidget>(
		GetWidgetFromName(TEXT("WidgetQuestLog"))
	);
	ensureAlways(QuestTextBlock != nullptr);
	ensureAlways(WidgetBossHpBar != nullptr);
	//작성 한승헌 - 2025 -11 - 10

	//APawn* Pawn = GetOwningPlayer() ? GetOwningPlayer()->GetPawn() : nullptr;

	//if (Pawn == nullptr)
	//{
	//	return;
	//}

	//UAbilitySystemComponent* ASC = nullptr;

	//if (Pawn->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
	//{
	//	ASC = IAbilitySystemInterface::GetAbilitySystemComponent(Pawn);
	//}

	//작성 종료
	
	//if (HpBar != nullptr && GetOwningPlayer() != nullptr)
	//{
	//	APawn* Pawn = GetOwningPlayer()->GetPawn();

	//	if (Pawn != nullptr)
	//	{
	//		//HpBar->SetAbilitySystemComponent(Pawn);
	//	}
	//}

	//MiniMap = Cast<UP4MiniMapWidget>(
	//	GetWidgetFromName(TEXT("WidgetMiniMap"))
	//);

	//ensureAlways(MiniMap);

	//IP4CharacterHUDInterface* HUDPawn
	//	= Cast<IP4CharacterHUDInterface>(GetOwningPlayer()->GetPawn());

	//if (HUDPawn != nullptr)
	//{
	//	HUDPawn->SetupHUDWidget(this);
	//}


	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	TArray<AActor*> FoundBosses;

	UGameplayStatics::GetAllActorsOfClass(world, AP4BossMonsterBase::StaticClass(), FoundBosses);

	if (FoundBosses.Num() > 0)
	{
		AActor* Boss = FoundBosses[0];

		if (WidgetBossHpBar)
		{
			WidgetBossHpBar->SetAbilitySystemComponent(Boss);
			WidgetBossHpBar->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Error, TEXT("[HUD] BossHpBar bind complete"));
		}
	}
}


void UP4HUDWidget::SetBoss(AActor* NewBoss)
{
	UE_LOG(LogTemp, Error, TEXT("SetBoss Activate"));
	if (WidgetBossHpBar == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BossHpBar Is Null"));

	}
	
	if (WidgetBossHpBar != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BossHpBar Is Not Null"));
		WidgetBossHpBar->SetAbilitySystemComponent(NewBoss);
		UE_LOG(LogTemp, Display, TEXT("Setting BossComplete."));
		WidgetBossHpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

//TObjectPtr<class UP4HpBarWidget> UP4HUDWidget::GetHpBar() const
//{
//	//return HpBar;
//}

TObjectPtr<class UP4HpAndShieldWidget> UP4HUDWidget::GetHpAndShieldBar() const
{
	return HpAndShieldBar;
}
