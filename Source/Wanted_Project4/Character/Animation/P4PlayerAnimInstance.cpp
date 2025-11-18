// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/P4PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "Character/P4CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"

UP4PlayerAnimInstance::UP4PlayerAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UP4PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UP4PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}

	AP4CharacterBase* Character = Cast<AP4CharacterBase>(GetOwningActor());
	if (Character)
	{
		if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(P4TAG_CHARACTER_ISDEAD))
		{
			bIsDead = true;
		}
		//todo: testcode
		else if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(P4TAG_CHARACTER_ISDAMAGED))
		{
			bIsDamaged = true;
		}
		else
		{
			bIsDead = false;
			bIsDamaged = false;
		}
	}
	
	// -작성: 노현기 -일시: 2025.11.18
	// 매 프레임 태그를 체크해서 변수 업데이트
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn);
		if (ASI)
		{
			UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (ASC)
			{
				FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
				bIsKatanaOnHand = ASC->HasMatchingGameplayTag(DrawnTag);
			}
		}
	}
}
