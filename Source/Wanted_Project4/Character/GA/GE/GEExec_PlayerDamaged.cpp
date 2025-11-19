// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/GE/GEExec_PlayerDamaged.h"
#include "Attribute/P4PlayerAttributeSet.h"

// Attribute 캡처 구조체
struct FStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageAmount);


	FStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4PlayerAttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4PlayerAttributeSet, Shield, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UP4PlayerAttributeSet, DamageAmount, Source, true);

	}
};

static FStatics& DamageStatics()
{
	static FStatics Statics;
	return Statics;
}

UGEExec_PlayerDamaged::UGEExec_PlayerDamaged()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);

	RelevantAttributesToCapture.Add(DamageStatics().DamageAmountDef);
}

void UGEExec_PlayerDamaged::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float PlayerHealth = 0.f;
	float PlayerShield = 0.f;
	float InDamage = 0.f;
	float ResultDamage = 0.f;

	// Attribute 캡처
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, {}, PlayerHealth);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, {}, PlayerShield);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageAmountDef, {}, InDamage);
	ResultDamage = InDamage;

	if (PlayerShield > 0)
	{
		float UsedShield = FMath::Min(PlayerShield, InDamage);
		ResultDamage -= UsedShield;

		// Shield 감소
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(DamageStatics().ShieldProperty, EGameplayModOp::Additive, -UsedShield)
		);
	}

	if (ResultDamage > 0)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -ResultDamage)
		);
	}




	// --- 실제 계산 로직 ---

	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
	//	DamageStatics().DamageAmountDef,
	//	FAggregatorEvaluateParameters(),
	//	InDamage
	//);

	//// Target의 CurHP에 반영
	//OutExecutionOutput.AddOutputModifier(
	//	FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -InDamage)
	//);
}
