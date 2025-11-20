// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/P4GA_Attack.h"
#include "Character/P4CharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
//#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P4GA_Jump.h"
//#include "Character/ABComboActionData.h"
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
		UE_LOG(LogTemp, Error, TEXT("[GA_StrongAttack] Super::CanActivateAbility 실패 - 태그 차단됨"));
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
	//CurrentComboData = P4Character->GetComboActionData();
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);



	//FVector ForwardDir = P4Character->GetActorForwardVector();
	//float RollDistance = 9000.f; // 구르기 거리
	//float RollDuration = 1.f;  // 구르기 시간

	//// 이동 속도 계산
	//FVector LaunchVelocity = ForwardDir * (RollDistance / RollDuration);

	//// 움직임 중 중력 무시
	//P4Character->LaunchCharacter(LaunchVelocity, true, false);

	UAnimMontage* DefaultAttackMontage = P4Character->GetDefaultAttackMontage();
	if (DefaultAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), DefaultAttackMontage,1.f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP4GA_Attack::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP4GA_Attack::OnInterruptedCallback);

		// AT의 함수 호출
		PlayAttackTask->ReadyForActivation();
	}
	

	//StartComboTimer();
}

FName UP4GA_Attack::GetNextSection()
{
	CurrentIndex = (CurrentIndex % MaxCount) + 1;

	//FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentIndex);
	FName NextSection = *FString::Printf(TEXT("DefaultAttack%d"), CurrentIndex);
	return NextSection;
}

void UP4GA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//UE_LOG(LogTemp, Log, TEXT("InputPressed"));

	//if (!ComboTimerHandle.IsValid())
	//{
	//	HasNextComboInput = false;
	//}
	//else
	//{
	//	HasNextComboInput = true;
	//}
}

void UP4GA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{

	UE_LOG(LogTemp, Warning, TEXT("[GA_Attack] CancelAbility 호출됨!"));

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UP4GA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AP4CharacterBase* P4Character = CastChecked<AP4CharacterBase>(ActorInfo->AvatarActor.Get());
	P4Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//CurrentComboData = nullptr;
	//CurrentCombo = 0;
	//HasNextComboInput = false;
}

void UP4GA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UP4GA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
