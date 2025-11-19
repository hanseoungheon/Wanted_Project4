// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/GA/AT/P4AT_Trace.h"
#include "Character/GA/TA/P4TA_Trace.h"

#include "Attribute/P4PlayerAttributeSet.h"
#include "Interface/P4DamageableInterface.h"

UP4GA_AttackHitCheck::UP4GA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP4GA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 이벤트 데이터 저장
	CurrentEventData = TriggerEventData; 

	// AbilityTask_Trace 생성 (UP4AT_Trace)
	UP4AT_Trace* AttackTraceTask = UP4AT_Trace::CreateTask(this, AP4TA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UP4GA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	UE_LOG(LogTemp, Log, TEXT("Begin"));
}

// 태스크 마무리 콜백
void UP4GA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	/*
	
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			// todo: 선우 documentation 정리( // 주석은 정리용임. 적용시킬 코드가 아님)
			//struct FGameplayAbilityTargetDataHandle
			//{
			//	TArray<TSharedPtr<FGameplayAbilityTargetData>> Data;
			//};
			//이라서 SweepMultiByChannel 결과가 여러 개 나올 경우 Data 배열 안에 각 HitResult가 TargetData 형태로 여러 개 들어가서 GAS가 자동으로 모든 타겟의 ASC를 순회하여 GE 각각 적용.

			//for (const TSharedPtr<FGameplayAbilityTargetData>& Data : TargetDataHandle.Data)
			//{
			//	for (TWeakObjectPtr<AActor> TargetActor : Data->GetActors())
			//	{
			//		TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
			//		TargetASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
			//	}
			//}
			
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

	*/


	// todo: 알아먹게 고치기 (여러마리 동시타격 지원)

	int32 NumData = TargetDataHandle.Data.Num();
	if (NumData == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No target data found."));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	const UP4PlayerAttributeSet* SourceAttribute = SourceASC->GetSet<UP4PlayerAttributeSet>();

	if (!SourceASC || !SourceAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("Source ASC or Attribute not found!"));
		return;
	}

	//const float AttackDamage = SourceAttribute->GetAttackRate();

	const float BaseAttackDamage = SourceAttribute->GetAttackRate();

	// AnimNotify에서 전달된 배율 가져오기
	float DamageMultiplier = 1.0f;
	if (CurrentEventData)
	{
		DamageMultiplier = CurrentEventData->EventMagnitude;
	}

	const float AttackDamage = BaseAttackDamage * DamageMultiplier;

	// 여러 타겟 순회
	for (int32 i = 0; i < NumData; ++i)
	{
		if (!UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, i))
			continue;

		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, i);
		AActor* HitActor = HitResult.GetActor();

		if (!HitActor || HitActor == GetAvatarActorFromActorInfo())
			continue;

		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *HitActor->GetName());

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target ASC not found for %s"), *HitActor->GetName());
			continue;
		}

		// AttributeSet 접근 (몬스터용)
		//UP4MonsterAttributeSet* TargetAttribute = const_cast<UP4MonsterAttributeSet*>(TargetASC->GetSet<UP4MonsterAttributeSet>());
		//if (!TargetAttribute)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Monster AttributeSet not found for %s"), *HitActor->GetName());
		//	continue;
		//}

		// 인터페이스 기반 데미지 처리
		if (IP4DamageableInterface* Monster = Cast<IP4DamageableInterface>(HitActor))
		{
			Monster->ApplyDamage(AttackDamage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s does not implement DamageableInterface"), *HitActor->GetName());
		}
	}

	// Ability 종료 처리
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
