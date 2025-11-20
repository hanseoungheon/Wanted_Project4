// Fill out your copyright notice in the Description page of Project Settings.


#include "P4CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Physics/P4Collision.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

// -작성: 노현기 -일시: 2025.11.10
// Todo: 인벤토리 컴포넌트 클래스 폴더 위치를 컴포넌트로 옮길 것
#include "Inventory/P4InventoryComponent.h"
#include "Item/ItemDataBase.h"
#include "UI/P4InventoryWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/P4GameplayTag.h"
#include "Item/Equipment/P4WeaponComponent.h"
#include "Inventory/P4EquipmentInvenComponent.h"

#include "Components/TimeLineComponent.h"

// Sets default values
AP4CharacterBase::AP4CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	// 
	// GAS 초기화
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UP4PlayerAttributeSet>(TEXT("AttributeSet"));

	// -작성: 노현기 -일시: 2025.11.10
	// 인벤토리 컴포넌트 생성
	InventoryComp = CreateDefaultSubobject<UP4InventoryComponent>(TEXT("InventoryComponent"));

	// -작성: 노현기 -일시: 2025.11.14
	// 무기 컴포넌트 생성
	WeaponComponent = CreateDefaultSubobject<UP4WeaponComponent>(TEXT("WeaponComponent"));

	// -작성: 노현기 -일시: 2025.11.18
	// 장비창 컴포넌트 생성
	EquipmentInvenComp = CreateDefaultSubobject<UP4EquipmentInvenComponent>(TEXT("EquipmentInvenComponent"));

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROPILE_P4CAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	//GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->MaxAcceleration = 800.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -38.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Set Assets
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
		TEXT("/Game/Character/Model/Hunter_King/King.King"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
		TEXT("/Game/Character/Animation/ABP_P4Character.ABP_P4Character_C"));
	if (AnimInstanceClassRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
	//if (ShoulderDataRef.Object)
	//{
	//	CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	//}

	//static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater'"));
	//if (QuaterDataRef.Object)
	//{
	//	CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultAttackMontageRef(TEXT("/Game/Character/Animation/Katana/ForUse/AM_KatanaDefaultAttack.AM_KatanaDefaultAttack"));
	if (DefaultAttackMontageRef.Object)
	{
		DefaultAttackMontage = DefaultAttackMontageRef.Object;
	}

	// -작성: 노현기 -일시: 2025.11.19
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StrongActionMontageRef(TEXT("/Game/Character/Animation/AM_StrongAttack.AM_StrongAttack"));
	if (StrongActionMontageRef.Object)
	{
		StrongAttackMontage = StrongActionMontageRef.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack'"));
	//if (ComboActionDataRef.Object)
	//{
	//	ComboActionData = ComboActionDataRef.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamagedMontageRef(TEXT("/Game/Character/Animation/Katana/ForUse/AM_KatanaDamaged.AM_KatanaDamaged"));
	if (DamagedMontageRef.Object)
	{
		DamagedMontage = DamagedMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/Character/Animation/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	//// Weapon Component
	//Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	////Weapon->SetupAttachment(GetMesh(), TEXT("hand_RSocket"));

	////todo: Load Weapon through Inventory
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(
	//	TEXT("/Game/Weapon/Katana.Katana"));
	//if (WeaponMeshRef.Succeeded())
	//{
	//	Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
	//}


	//작성 - 한승헌
	//일시- 2025.11.19
	//내용 - 타임라인, 커브들 로드.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> RollingCurveRef(TEXT("/Game/Character/Curve/CV_RollingCurve.CV_RollingCurve"));

	if (RollingCurveRef.Succeeded() == true)
	{
		RollingCurve = RollingCurveRef.Object;
	}

	//타임라인 생성
	RollingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollingTimeLine"));
}

#include "GameplayTagContainer.h"
// todo: 죽음과 피격 몽타주로 빼면..
void AP4CharacterBase::ApplyDamage(const float DamageAmount)
{
	// todo: 알아먹게 수정
	if (ASC)
	{
		//작성-한승헌
		//일시- 2025.11.20
		//내용- 구르기시 무적상태. 
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.IsRolling"))))
		{
			return;
		}
		
		// todo: Hit 몽타주, 넉백 같은 즉각 반응 -> ABP에서 처리
		//DamagedActionBegin();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//AnimInstance->StopAllMontages(0.f);
		}

		// 1. 취소하고 싶은 태그를 컨테이너에 담습니다.
		FGameplayTagContainer TargetTags;
		TargetTags.AddTag(P4TAG_CHARACTER_ISATTACKING);

		// 2. CancelAbilities에 컨테이너의 주소를 넘깁니다.
		// 매개변수 순서: (포함된 태그, 제외할 태그, 무시할 어빌리티)
		ASC->CancelAbilities(&TargetTags, nullptr, nullptr);

		UE_LOG(LogTemp, Warning, TEXT("PLAYER DAMAGED"))
		if (ASC->HasMatchingGameplayTag(P4TAG_CHARACTER_ISDAMAGED) == false)
		{
			ASC->AddLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);
		}

		// todo: 임시 수정 점프 상태 제외 Damaged 모션동안 이동 막기
		if (ASC->HasMatchingGameplayTag(P4TAG_CHARACTER_ISJUMPING) == false)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_None);
		}
		//GetCharacterMovement()->SetMovementMode(MOVE_None);

		// 받을 데미지 설정
		AttributeSet->SetDamageAmount(DamageAmount);

		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		Context.AddSourceObject(this);

		// BPGE_PlayerDamaged 블루프린트 GameplayEffect (서버)
		FSoftClassPath GEPath(TEXT("/Game/Character/GE/BPGE_PlayerDamaged.BPGE_PlayerDamaged_C"));
		TSoftClassPtr<UGameplayEffect> DamagedEffectSoftClass(GEPath);
		if (DamagedEffectSoftClass.IsPending())
		{
			DamagedEffectSoftClass.LoadSynchronous();
		}

		if (TSubclassOf<UGameplayEffect> DamagedEffectClass = DamagedEffectSoftClass.Get())
		{
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamagedEffectClass, 1.f, Context);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}
void AP4CharacterBase::GiveDamage(AActor* TargetActor, const float DamageAmount)
{
	// GA로 처리함.
}

void AP4CharacterBase::OnRollingUpdate(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRollingUpdate: %f"), Value);
	Rolling();
}

void AP4CharacterBase::Rolling()
{
	const float KeepZ = GetCharacterMovement()->IsMovingOnGround() ? 0.0f 
		: GetCharacterMovement()->GetGravityZ();

	FVector Dir = RollingDirection;

	const float RollingSpeed = 800.0f; //구르기 속도.

	Dir.Normalize(); // 혹시 모르니


	GetCharacterMovement()->Velocity = (Dir * RollingSpeed);
	GetCharacterMovement()->Velocity.Z = KeepZ;
}

void AP4CharacterBase::HandleRespawn()
{
	if (!ASC) return; // 죽음 태그 제거 
	ASC->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);
	ASC->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDEAD);
	// 체력 회복 
	ASC->SetNumericAttributeBase(AttributeSet->GetHealthAttribute(), AttributeSet->GetMaxHealth());
	ASC->SetNumericAttributeBase(AttributeSet->GetShieldAttribute(), AttributeSet->GetMaxShield());
	// 콜리전 복구 
	SetActorEnableCollision(true); 
	// 이동 가능 
	GetCharacterMovement()->SetMovementMode(MOVE_Walking); 
	// 위치 초기화 (원하는 리스폰 포인트) 
	SetActorTransform(RespawnTransform);
}

void AP4CharacterBase::DamagedActionBegin()
{
	// Damaged 몽타주 실행
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		ASC->AddLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);

		// Damaged 모션동안 이동 막기
		GetCharacterMovement()->SetMovementMode(MOVE_None);

		// Damaged 몽타주 재생
		// todo: 임시 피격 애니메이션 속도 *3
		AnimInstance->Montage_Play(DamagedMontage, 2.5f);

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AP4CharacterBase::DamagedActionEnd);

		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, DamagedMontage);
	}
}

void AP4CharacterBase::DamagedActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	ASC->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AP4CharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
        //AttributeSet = NewObject<UP4PlayerAttributeSet>(this, UP4PlayerAttributeSet::StaticClass());
    }
	UP4PlayerAttributeSet* PlayerAttributeSet = Cast<UP4PlayerAttributeSet>(AttributeSet);
	PlayerAttributeSet->OnHpZero.AddUObject(this, &AP4CharacterBase::SetDead);
}

void AP4CharacterBase::SetDead()
{
	//todo: 무기 가지고 죽으면 인벤토리에 넣고 일반상태로 변경
	if (ASC)
	{
		// 모든 Tag 해제
		//ASC->RemoveLooseGameplayTags(ASC->GetOwnedGameplayTags());
		// Dead Tag 부착
		//ASC->AddLooseGameplayTag(P4TAG_CHARACTER_ISDEAD);

		// ASC가진 액터에 태그 부착(Character.State.IsDead) 해서 이 태그가 트리거 태그로 지정된 BPGA_Death 실행.
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->StopAllMontages(0.f);
		}
		
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, P4TAG_CHARACTER_ISDEAD, PayloadData);
	}	

	// 이동 못하게 막기 -> GA_Death에서 처리
	//GetCharacterMovement()->SetMovementMode(MOVE_None);

	// 사망 몽타주 재생 -> ABP에서 몽타주 말고 시퀀스로 처리
	// todo: 몽타주 관련 삭제
	//PlayDeadAnimation();

	// 콜리전 끄기 -> GA_Death에서 처리
	//SetActorEnableCollision(false);

	// DeadEventDelayTime 후 액터 삭제
	//FTimerHandle DeadTimerHandle;
	//float DeadEventDelayTime = 5.f;
	//GetWorld()->GetTimerManager().SetTimer(
	//	DeadTimerHandle,
	//	[&]()
	//	{
	//		Destroy();
	//	},
	//	DeadEventDelayTime,
	//	false
	//);
}

void AP4CharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);

		const float PlayRate = 2.0f;
		AnimInstance->Montage_Play(DeadMontage, PlayRate);
	}
}

//작성 : 한승헌
//일시 : 2025.11.19
//내용 : 구르기 전용 인터페이스.
FVector AP4CharacterBase::GetRollingDirection() const
{
	return RollingDirection;
}

void AP4CharacterBase::SetRollingDirection(FVector InRollingDirection)
{
	RollingDirection = InRollingDirection;
}

void AP4CharacterBase::StartRollingTimeLine()
{
	RollingTimeLine->PlayFromStart();
	UE_LOG(LogTemp, Warning, TEXT("RollingTimeLineStart"));
}

// -작성: 노현기 -일시: 2025.11.10
void AP4CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	//작성- 한승헌
	//일시- 2025.11.19
	//내용 - 타임라인들 추가.
	if (RollingCurve != nullptr)
	{
		FOnTimelineFloat RollingInterpFunction;

		RollingInterpFunction.BindUFunction(this, FName("OnRollingUpdate"));
		RollingTimeLine->AddInterpFloat(RollingCurve, RollingInterpFunction);

		//반복하지 않음.
		RollingTimeLine->SetLooping(false);
		RollingTimeLine->SetPlayRate(1.0f);
		//RollingTimeLine->PlayFromStart();
	}


	// 테스트용 아이템 추가 (UI는 컨트롤러가 담당)
	if (InventoryComp)
	{
		UItemDataBase* TestSword = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Equipment/DA_Sword.DA_Sword"));
		UItemDataBase* TestShield = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Equipment/DA_Shield.DA_Shield"));
		UItemDataBase* TestPotion = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Consumable/DA_HealthPotion.DA_HealthPotion"));
		UItemDataBase* TestPotion2 = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Consumable/DA_IncreaseMaxHealthPotion.DA_IncreaseMaxHealthPotion"));

		//작성-한승헌
		//일시- 2025.11.18
		//내용- 강화석 강화책 추가.
		UItemDataBase* MagicBook = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Upgrades/DA_MagicBook.DA_MagicBook"));
		UItemDataBase* MagicStone = LoadObject<UItemDataBase>(nullptr,
			TEXT("/Game/Item/Upgrades/DA_MagicStone.DA_MagicStone"));

		if (TestSword)
		{
			InventoryComp->AddItem(TestSword, 2);
		}

		if (TestPotion)
		{
			InventoryComp->AddItem(TestPotion, 10);
			InventoryComp->AddItem(TestPotion, 5);
			InventoryComp->AddItem(TestPotion, 99);
			InventoryComp->AddItem(TestPotion, 90);
			InventoryComp->AddItem(TestPotion2, 50);
		}

		//작성-한승헌
		//일시- 2025.11.18
		//내용- 강화석 강화책 추가.
		if (MagicBook != nullptr)
		{
			InventoryComp->AddItem(MagicBook, 1);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Is MagicBook NULLPTR"));
		}

		if (MagicStone != nullptr)
		{
			InventoryComp->AddItem(MagicStone, 1);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Is MagicStone NULLPTR"));
		}
	}
}

void AP4CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RollingTimeLine)
	{
		RollingTimeLine->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

UAbilitySystemComponent* AP4CharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

//void AP4CharacterBase::AttackHitCheck()
//{
//	// 충돌 판정 로직 작성.
//}


