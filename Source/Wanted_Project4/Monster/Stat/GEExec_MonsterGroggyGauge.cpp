// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Stat/GEExec_MonsterGroggyGauge.h"

#include "P4MonsterAttributeSet.h"

struct FGroggyGaugeStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageAmount);
	DECLARE_ATTRIBUTE_CAPTUREDEF(GroggyGauge);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHP);

	FGroggyGaugeStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4MonsterAttributeSet, DamageAmount, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4MonsterAttributeSet, GroggyGauge, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4MonsterAttributeSet, MaxHP, Source, true);
	}
};

static const FGroggyGaugeStatics& GroggyGaugeStatics()
{
	static FGroggyGaugeStatics GroggyGaugeStatics;
	return GroggyGaugeStatics;
}

// AttributeSet->GetGroggyGauge()
// 				+ (DamageAmount / (AttributeSet->GetMaxHP())) * 300.f;


UGEExec_MonsterGroggyGauge::UGEExec_MonsterGroggyGauge()
{
	RelevantAttributesToCapture.Add(GroggyGaugeStatics().DamageAmountDef);
	RelevantAttributesToCapture.Add(GroggyGaugeStatics().GroggyGaugeDef);
	RelevantAttributesToCapture.Add(GroggyGaugeStatics().MaxHPDef);
}

void UGEExec_MonsterGroggyGauge::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	float Damage = 0.f;
	float MaxHP = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GroggyGaugeStatics().DamageAmountDef,
		FAggregatorEvaluateParameters(),
		Damage
	);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GroggyGaugeStatics().MaxHPDef,
		FAggregatorEvaluateParameters(),
		MaxHP
	);

	float GroggyGauge = (Damage / MaxHP) * 300.f;

		// 체력 감소 적용
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
				GroggyGaugeStatics().GroggyGaugeProperty,
				EGameplayModOp::Additive,
				GroggyGauge
			)
		);

	UE_LOG(LogTemp, Log, TEXT("Groggy 적용: -%f"), GroggyGauge);
}
