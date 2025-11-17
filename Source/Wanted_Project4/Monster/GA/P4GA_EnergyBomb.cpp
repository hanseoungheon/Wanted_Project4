// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/P4GA_EnergyBomb.h"

#include "Monster/P4BossMonsterBase.h"

UP4GA_EnergyBomb::UP4GA_EnergyBomb()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Monster.Action.EnergyBomb")));
}

void UP4GA_EnergyBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// @Todo: Ability 호출 확인
	UE_LOG(LogTemp, Log, TEXT("EnergyBomb 호출"));
	
	// 몬스터 베이스 캐스트
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(ActorInfo->AvatarActor.Get());
	if (Monster)
	{
		FName MontageSectionName = TEXT("EnergyBomb");
		// 몽타주 재생
		Monster->AttackActionBegin(MontageSectionName, 2.6f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
