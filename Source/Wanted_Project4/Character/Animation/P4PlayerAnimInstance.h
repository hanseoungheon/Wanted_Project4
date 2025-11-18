// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P4PlayerAnimInstance.generated.h"

/**
 * 
 */
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDamaged : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

public:
	// -작성자: 노현기 -일시: 2025.11.13
	// 카타나가 캐릭터에게 부착 되었는지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsEquipped;

	// -작성자: 노현기 -일시: 2025.11.14
	// 캐릭터가 발도를 해서 카타나를 손에 쥐고 있는지 ('V'키로 토글)
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsKatanaOnHand;

};
