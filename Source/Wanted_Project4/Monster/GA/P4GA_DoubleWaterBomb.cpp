// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/P4GA_DoubleWaterBomb.h"

#include "Monster/P4BossMonsterBase.h"

UP4GA_DoubleWaterBomb::UP4GA_DoubleWaterBomb()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Monster.Action.DoubleWaterBomb")));
	SetAssetTags(Tags);
}

void UP4GA_DoubleWaterBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// @Todo: Ability 호출 확인
	UE_LOG(LogTemp, Log, TEXT("Double Water Bomb 호출"));
	
	// 몬스터 베이스 캐스트
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(ActorInfo->AvatarActor.Get());
	if (Monster)
	{
		FName MontageSectionName = TEXT("DoubleWaterBomb");
		// 몽타주 재생
		Monster->AttackActionBegin(MontageSectionName, 1.3f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
