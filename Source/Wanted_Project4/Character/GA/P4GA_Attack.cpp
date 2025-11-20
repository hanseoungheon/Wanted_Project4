// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Attack.h"
#include "Character/P4CharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Character/P4CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Tag/P4GameplayTag.h"

UP4GA_Attack::UP4GA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP4GA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 부모 클래스의 태그 검증 먼저 (블루프린트에서 설정한 태그들 체크)
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		UE_LOG(LogTemp, Error, TEXT("[GA_Attack] Super::CanActivateAbility 실패 - 태그 차단됨"));
		return false;
	}

	// 추가 검증: 무기가 장착되어 있는지 확인
	const AP4CharacterPlayer* Player = Cast<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (!Player)
	{
		return false;
	}

	USkeletalMeshComponent* MeshComp = Player->GetMesh();
	const UP4PlayerAnimInstance* AnimInst = MeshComp ? Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;

	if (!AnimInst)
	{
		return false;
	}

	// 무기가 장착되어 있는지 확인
	if (!AnimInst->CharacterState.bIsEquipped)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GA_Draw] 무기가 장착되지 않았습니다!"));
		return false;
	}

	// 손에 칼이 없으면 공격 불가
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
	if (ASC)
	{
		FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
		if (!ASC->HasMatchingGameplayTag(DrawnTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("[GA_Attack] 손에 칼이 없습니다!"));
			return false;
		}
	}

	return true;
}

void UP4GA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// ★ 1. 피격 태그 감지 Task 추가
	UAbilityTask_WaitGameplayTagAdded* WaitDamaged = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, P4TAG_CHARACTER_ISDAMAGED);

	WaitDamaged->Added.AddDynamic(this, &UP4GA_Attack::OnInterruptedCallback);
	WaitDamaged->ReadyForActivation();


	UAnimMontage* DefaultAttackMontage = P4Character->GetDefaultAttackMontage();
	if (DefaultAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), DefaultAttackMontage, 1.f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP4GA_Attack::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP4GA_Attack::OnInterruptedCallback);

		// AT의 함수 호출
		PlayAttackTask->ReadyForActivation();
	}
}

FName UP4GA_Attack::GetNextSection()
{
	CurrentIndex = (CurrentIndex % MaxCount) + 1;

	FName NextSection = *FString::Printf(TEXT("DefaultAttack%d"), CurrentIndex);
	return NextSection;
}

void UP4GA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP4GA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UP4GA_Attack::OnInterruptedCallback()
{
	// ★ 1. 몽타주 강제 정지
	if (UAnimInstance* Anim = CurrentActorInfo->GetAnimInstance())
	{
		Anim->StopAllMontages(0.f);
	}

	// ★ 2. Ability 강제 종료
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}
