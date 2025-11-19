// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P4PlayerAnimInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FP4CharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsIdle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDamaged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKatanaOnHand = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRunning = false;
};

UCLASS()
class WANTED_PROJECT4_API UP4PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UP4PlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDamaged : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;

public:
	//// -작성자: 노현기 -일시: 2025.11.13
	//// 카타나가 캐릭터에게 부착 되었는지
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//bool bIsEquipped : 1;

	//// -작성자: 노현기 -일시: 2025.11.14
	//// 캐릭터가 발도를 해서 카타나를 손에 쥐고 있는지 ('V'키로 토글)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//bool bIsKatanaOnHand : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	FP4CharacterState CharacterState;

	//UFUNCTION(BlueprintCallable, Category = "State")
	//const FP4CharacterState& GetCharacterState() const { return CharacterState; }

	UFUNCTION()
	void OnTagChanged(const FGameplayTag Tag, int32 NewCount);
};
