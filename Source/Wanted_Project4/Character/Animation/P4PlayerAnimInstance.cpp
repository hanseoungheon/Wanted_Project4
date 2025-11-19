// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/P4PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "Character/P4CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"

UP4PlayerAnimInstance::UP4PlayerAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UP4PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	AP4CharacterBase* P4Character = Cast<AP4CharacterBase>(Owner);
	if (!P4Character)
		return;
	UAbilitySystemComponent* ASC = P4Character->GetAbilitySystemComponent();
	if (!ASC) return;

	//ASC->RegisterGameplayTagEvent(P4TAG_CHARACTER_ISJUMPING).AddUObject(this, &UP4PlayerAnimInstance::OnTagChanged);

	ASC->RegisterGameplayTagEvent(P4TAG_CHARACTER_ISDAMAGED,EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UP4PlayerAnimInstance::OnTagChanged);
	ASC->RegisterGameplayTagEvent(P4TAG_CHARACTER_ISDEAD,EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UP4PlayerAnimInstance::OnTagChanged);
	ASC->RegisterGameplayTagEvent(P4TAG_CHARACTER_ISDRAWN,EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UP4PlayerAnimInstance::OnTagChanged);
	ASC->RegisterGameplayTagEvent(P4TAG_CHARACTER_ISRUNNING,EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UP4PlayerAnimInstance::OnTagChanged);
}

void UP4PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		CharacterState.bIsIdle = GroundSpeed < MovingThreshold;
		CharacterState.bIsFalling = Movement->IsFalling();
		CharacterState.bIsJumping = CharacterState.bIsFalling && (Velocity.Z > JumpingThreshold);
	}

	//AP4CharacterBase* Character = Cast<AP4CharacterBase>(GetOwningActor());
	//if (Character)
	//{
	//	if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(P4TAG_CHARACTER_ISDEAD))
	//	{
	//		bIsDead = true;
	//	}
	//	//todo: testcode
	//	else if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(P4TAG_CHARACTER_ISDAMAGED))
	//	{
	//		bIsDamaged = true;
	//	}
	//	else
	//	{
	//		bIsDead = false;
	//		bIsDamaged = false;
	//	}
	//}
	
	// -작성: 노현기 -일시: 2025.11.18
	// 매 프레임 태그를 체크해서 변수 업데이트
	//APawn* Pawn = TryGetPawnOwner();
	//if (Pawn)
	//{
	//	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn);
	//	if (ASI)
	//	{
	//		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	//		if (ASC)
	//		{
	//			FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
	//			bIsKatanaOnHand = ASC->HasMatchingGameplayTag(DrawnTag);
	//		}
	//	}
	//}
}

void UP4PlayerAnimInstance::OnTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bool bAdded = (NewCount > 0);

	if (Tag.MatchesTagExact(P4TAG_CHARACTER_ISDEAD))
		CharacterState.bIsDead = bAdded;

	else if (Tag.MatchesTagExact(P4TAG_CHARACTER_ISDAMAGED))
		CharacterState.bIsDamaged = bAdded;

	else if (Tag.MatchesTagExact(P4TAG_CHARACTER_ISDRAWN))
		CharacterState.bIsKatanaOnHand = bAdded;

	else if (Tag.MatchesTagExact(P4TAG_CHARACTER_ISRUNNING))
		CharacterState.bIsRunning = bAdded;
}