// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/P4GA_LeftWingStomp.h"

#include "Monster/P4BossMonsterBase.h"
#include "Monster/P4MonsterBase.h"

UP4GA_LeftWingStomp::UP4GA_LeftWingStomp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// 단순히 AddTag의 경우에는 AbilitySped 에 반영되지 않음
	// 그로인해 TryActivateAbilitiesByTag 가 태그로 AbilitySpec 을 찾지 못함
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Monster.Action.LeftWingStomp")));
	SetAssetTags(Tags);
}

void UP4GA_LeftWingStomp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// @Todo: Ability 호출 확인
	UE_LOG(LogTemp, Log, TEXT("LeftWingStomp Ability 호출"));

	// 몬스터 베이스 캐스트
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(ActorInfo->AvatarActor.Get());
	if (Monster)
	{
		FName MontageSectionName = TEXT("LeftWingStomp");
		// 몽타주 재생
		Monster->AttackActionBegin(MontageSectionName, 2.6f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
