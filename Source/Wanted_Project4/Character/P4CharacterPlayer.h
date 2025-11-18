// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P4CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/P4CharacterHUDInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Game/P4UpgradeType.h"
#include "P4CharacterPlayer.generated.h"
/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4CharacterPlayer : public AP4CharacterBase,
	public IP4CharacterHUDInterface
{
	GENERATED_BODY()

public:
	AP4CharacterPlayer();

	// 컨트롤러가 호출하는 함수들
public:
	// 입력 제어 함수
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	// 입력 처리 함수
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleSuicide(const FInputActionValue& Value);

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	//작성 한승헌
	//일시- 2025.11.17
	//무기강화를 위한 함수. -강화당하기.
	UFUNCTION(BlueprintCallable, Category = Enchant)
	void ApplyEnchantWeapon(float InRate, EP4UpgradeType UpgradeType);

	//작성- 한승헌 
	//일시- 2025.11.14 
	// 캐릭터가 인터렉트하고있는 액터.
	TObjectPtr<AActor> CurrentInteractActor;

	// -작성자: 노현기 -일시: 2025.11.14
	FORCEINLINE class UAnimMontage* GetDrawKatanaMontage() const { return DrawKatanaMontage; }
	FORCEINLINE class UAnimMontage* GetSheathKatanaMontage() const { return SheathKatanaMontage; }
	
protected:
	// components.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	//MapComponents
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MiniMap, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> MapSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MiniMap, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneCaptureComponent2D> MapViewrCapture;

	//TObjectPtr<class UTextureRenderTarget2D> MiniMapTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MiniMap, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSprite> CompassSprite;
protected:
	///Script/Engine.AnimMontage'/Game/Character/Animation/AM_Rolling.AM_Rolling'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollingMontage;

	///Script/Engine.AnimMontage'/Game/Character/Animation/AM_Rolling.AM_Rolling'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RunMontage;

	// -작성자: 노현기 -일시: 2025.11.14
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DrawKatanaMontage;

	// -작성자: 노현기 -일시: 2025.11.15
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SheathKatanaMontage;
public:
	// GAS
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	// 강화에 사용할 GE 클래스 (P4GE_EnchantWeapon BP 지정)
	UPROPERTY(EditDefaultsOnly, Category = Enchant)
	TSubclassOf<class UGameplayEffect> EnchantEffectClass;

	// 현재 적용 중인 강화 GE 핸들 (중복 적용 막고 갱신용)
	FActiveGameplayEffectHandle CurrentEnchantEffectHandle;

private:
	//UPROPERTY(EditAnywhere, Category = Weapon)
	//TObjectPtr<class USkeletalMesh> WeaponMesh;


//HUD/UI 섹션
protected:
	//virtual void SetupHUDWidget(class UP4HUDWidget* InHudWidtet) override;
};	
