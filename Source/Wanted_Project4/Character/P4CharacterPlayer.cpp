// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4CharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PaperSprite.h"
#include "Engine/Texture2D.h"
#include "Animation/AnimMontage.h"
#include "GameplayEffect.h"
#include "Game/P4UpgradeType.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Tag/P4GameplayTag.h"

AP4CharacterPlayer::AP4CharacterPlayer()
{
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;
	//bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	//GetCharacterMovement()->JumpZVelocity = 300.0f;
	//
	//// 
	//GetMesh()->SetRelativeLocationAndRotation(
	//	FVector(0.0f, 0.0f, -38.0f),
	//	FRotator(0.0f, -90.0f, 0.0f)
	//);

	//// Set Assets
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
	//	TEXT("/Game/Character/Model/Hunter_King/King.King"));
	//if (CharacterMesh.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	//}

	//static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(
	//	TEXT("/Game/Character/Animation/ABP_P4Character.ABP_P4Character_C"));
	//if (CharacterAnim.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(CharacterAnim.Class);
	//}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	
	//미니맵을 위한 코드입니다.
	//-작성: 한승헌 -일시: 2025.11.07
	MapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMap SpringArm"));
	MapSpringArm->SetupAttachment(RootComponent);
	MapSpringArm->TargetArmLength = 300.0f;
	MapSpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	MapSpringArm->bUsePawnControlRotation = false;  // 컨트롤러 회전 영향 X
	MapSpringArm->bDoCollisionTest = false;         // 지형에 밀려 짧아지는 것 방지
	/*Cast<USceneComponent>MapSpringArm->bAbsoluteRotation = true;  */       // 월드 고정 회전

	MapViewrCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCapture"));
	MapViewrCapture->SetupAttachment(MapSpringArm);
	MapViewrCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MapViewrCapture->OrthoWidth = 32768.0f;
	MapViewrCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorHDR;

	//작성: 한승헌 -일시 : 2025.11.11
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapRef(TEXT("/Game/UI/MiniMap/MiniMapRenderTarget.MiniMapRenderTarget"));

	if (MiniMapRef.Succeeded() == true)
	{
		MapViewrCapture->TextureTarget = MiniMapRef.Object;
	}


	CompassSprite = CreateDefaultSubobject<UPaperSprite>(TEXT("CompassSprite"));
	//CompassSprite->SetupAttachment(RootComponent);

	//작성: 한승헌 - 일시: 2025.11.12
	static ConstructorHelpers::FObjectFinder<UTexture2D> CompassTextureRef(TEXT("/Game/UI/MiniMap/Compass.Compass"));
	
	if (CompassTextureRef.Succeeded())
	{
		//CompassSprite->GetSourceTexture() = CompassTextureRef.Object;
	}
	//여기까지가 미니맵 코드입니다. - 작성: 한승헌.


	//작성-한승헌
	//일시- 2025.11.14
	//현재 인터렉스하고 있는 액터 포인터 초기화.
	CurrentInteractActor = nullptr;

	//작성-한승헌
	//일시 - 2025.11.14
	//무기강화 GE 경로지정.
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EnchantEffectClassRef(TEXT("/Game/NPC/GE/BPGE_EnchantWeapon.BPGE_EnchantWeapon_C"));

	if (EnchantEffectClassRef.Succeeded() == true)
	{
		EnchantEffectClass = EnchantEffectClassRef.Class;
	}

	// -작성: 노현기 -일시: 2025.11.14
	// 발도 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DrawKatanaRef(
		TEXT("/Game/Character/Animation/AM_DrawKatana.AM_DrawKatana"));
	if (DrawKatanaRef.Succeeded())
	{
		DrawKatanaMontage = DrawKatanaRef.Object;
	}

	// -작성: 노현기 -일시: 2025.11.15
	// 납도 몽타주 로드 
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SheathKatanaRef(
		TEXT("/Game/Character/Animation/AM_SheathKatana.AM_SheathKatana"));
	if (SheathKatanaRef.Succeeded())
	{
		SheathKatanaMontage = SheathKatanaRef.Object;
	}
}

void AP4CharacterPlayer::GASInputPressed(int32 InputId)
{
	if (!ASC) return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;

		// 어빌리티 발동 중인가?
		if (Spec->IsActive())
		{
			// 발동 중에 입력 들어옴
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AP4CharacterPlayer::GASInputReleased(int32 InputId)
{
	if (!ASC) return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}


void AP4CharacterPlayer::HandleMove(const FInputActionValue& Value)
{
	// 입력 값 읽어오기.
	FVector2D Movement = Value.Get<FVector2D>();

	// 이동할 방향 만들기.
	// 카메라가 바라보는 방향(컨트롤러의 방향)을 기준으로 이동 방향 만들기.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 앞방향.
	FVector ForwardVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 오른쪽 방향.
	FVector RightVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트에 입력 전달하기.
	// 크기가 1인 숫자를 곱하는 행위는 "회전"
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);
}

void AP4CharacterPlayer::HandleLook(const FInputActionValue& Value)
{
	// 입력 값 읽어오기.
	FVector2D LookValue = Value.Get<FVector2D>();

	// 컨트롤러에 회전 적용.

	// 마우스 좌우 드래그 입력을 컨트롤러의 Z축 회전(요, Yaw)에 적용.
	AddControllerYawInput(LookValue.X);

	// 마우스 좌우 드래그 입력을 컨트롤러의 Y축 회전(피치, Pitch)에 적용.
	AddControllerPitchInput(LookValue.Y);
}

void AP4CharacterPlayer::HandleSuicide(const FInputActionValue& Value = FInputActionValue::Axis1D())
{
	if (!ASC)
	{
		return;
	}
	

	// 중복 실행 차단
	if (GetAttributeSet()->GetHealth() <= 0)
	{
		return;
	}
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	// todo: 밑의 코드는 자기 자신만 사용하는거라 필요없다??
	//Context.AddSourceObject(this);

	// Suicide GE Load
	FSoftClassPath GEPath(TEXT("/Game/Character/GE/BPGE_PlayerSuicide.BPGE_PlayerSuicide_C"));
	TSoftClassPtr<UGameplayEffect> SuicideEffect(GEPath);

	if (SuicideEffect.IsPending())
	{
		SuicideEffect.LoadSynchronous();
	}

	if (TSubclassOf<UGameplayEffect> GEClass = SuicideEffect.Get())
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GEClass, 1.f, Context);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AP4CharacterPlayer::ApplyEnchantWeapon(float InRate, EP4UpgradeType UpgradeType)
{
	if (ASC == nullptr || EnchantEffectClass == nullptr)
	{
		return;
	}

	//// 2) 기존 강화 효과 제거(있으면)
	//if (CurrentEnchantEffectHandle.IsValid())
	//{
	//	ASC->RemoveActiveGameplayEffect(CurrentEnchantEffectHandle);
	//	CurrentEnchantEffectHandle.Invalidate();
	//}


	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle 
		= ASC->MakeOutgoingSpec(EnchantEffectClass, 1.0f, Context);

	if (SpecHandle.IsValid() == true)
	{
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		if (Spec == nullptr)
		{
			return;
		}

		FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Data.Weapon.Attack"));
		FGameplayTag MaxHpTag = FGameplayTag::RequestGameplayTag(FName("Data.Weapon.MaxHealth"));

		float AttackValue = 0.f;
		float MaxHpValue = 0.f;

		if (UpgradeType == EP4UpgradeType::Attack)
		{
			AttackValue = InRate;
		}
		else if (UpgradeType == EP4UpgradeType::MaxHealth)
		{
			MaxHpValue = InRate;
		}

		Spec->SetSetByCallerMagnitude(AttackTag, AttackValue);
		Spec->SetSetByCallerMagnitude(MaxHpTag, MaxHpValue);

		CurrentEnchantEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec);

	}
}

//void AP4CharacterPlayer::SetupHUDWidget(UP4HUDWidget* InHudWidtet)
//{
//	if (InHudWidtet != nullptr)
//	{
//		
//	}
//}

//void AP4CharacterPlayer::HandleAttack(const FInputActionValue& Value)
//{
//	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
//	{
//		ASC->TryActivateAbilityByClass(UBasicAttackAbility::StaticClass());
//	}
//}

void AP4CharacterPlayer::FellOutOfWorld(const UDamageType& dmgType)
{
	//Super::FellOutOfWorld(dmgType);

	// 여기서 바로 죽음 처리
	ASC->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISJUMPING);
	HandleSuicide();   // 네가 만든 것
}