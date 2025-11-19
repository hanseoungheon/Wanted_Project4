// Fill out your copyright notice in the Description page of Project Settings.


#include "P4MonsterAttributeSet.h"
#include "GameplayEffectExtension.h"

UP4MonsterAttributeSet::UP4MonsterAttributeSet()
{
	DamageAmount = 0.f;
}

// 게임플레이 이펙트 적용 후에 호출되는 함수
void UP4MonsterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// CurHP 가 변경되었을 때 처리
	if (Data.EvaluatedData.Attribute == GetCurHPAttribute())
	{
		// MaxHP 초과 방지
		float NewHP = FMath::Clamp(GetCurHP(), 0.f, GetMaxHP());
		SetCurHP(NewHP);

		// 몬스터가 죽었는지 체크
		if (NewHP <= 0.f)
		{
			// 어딘가에 OnHpZeroDelegate 델리게이트 생성

			// 사망 함수 만들고 바인드
			
			// 여기서 몬스터 사망 이벤트 broadcast 등등
			OnHpZero.Broadcast();
		}
	}

	if (Data.EvaluatedData.Attribute == GetGroggyGaugeAttribute())
	{
		float NewGroggyGauge = FMath::Clamp(GetGroggyGauge(), 0.f, 100.f);
		SetGroggyGauge(NewGroggyGauge);

		UE_LOG(LogTemp, Log, TEXT("[Monster Attribute Set] Current Groggy Gauge = %f"), NewGroggyGauge);

		if (NewGroggyGauge >= 100.f)
		{
			OnGroggyStart.Broadcast();
		}
	}
}
