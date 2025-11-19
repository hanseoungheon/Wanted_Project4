// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/P4GA_DashAttack.h"

#include "Monster/P4BossMonsterBase.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/Character.h"

UP4GA_DashAttack::UP4GA_DashAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Monster.Action.DashAttack")));
	SetAssetTags(Tags);
}

void UP4GA_DashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// @Todo: Ability 호출 확인
	UE_LOG(LogTemp, Log, TEXT("DashAttack 호출"));

	// 몬스터 베이스 캐스트
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(ActorInfo->AvatarActor.Get());
	if (Monster)
	{
		Monster->OnDashStart.AddDynamic(this, &UP4GA_DashAttack::StartDashForce);
		
		FName MontageSectionName = TEXT("DashAttack");
		// 몽타주 재생
		Monster->AttackActionBegin(MontageSectionName, 2.2f);
	}


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UP4GA_DashAttack::StartDashForce()
{
	AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(GetAvatarActorFromActorInfo());
	if (Monster)
	{
		// @MobTODO: 돌진 판정 확인
		UE_LOG(LogTemp, Log, TEXT("StartDashForce"));
		
		auto* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			NAME_None,
			Monster->GetActorForwardVector(),
			2000.f, /*DashSpeed*/
			0.5f, /*DashDuration*/
			false,
			nullptr,
			ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
			FVector::ZeroVector,
			0.f,
			false
		);

		Task->ReadyForActivation();
	}
}
