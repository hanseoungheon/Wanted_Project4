// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/P4PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Character/P4CharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "UI/P4HUDWidget.h"
#include "UI/P4HpBarWidget.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Inventory/P4InventoryComponent.h"
#include "UI/P4InventoryWidget.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "Inventory/P4EquipmentInvenComponent.h"
#include "UI/P4EquipmentInvenWidget.h"
#include "Components/Border.h"

AP4PlayerController::AP4PlayerController()
{
	// 게임 시작 시 커서 숨김
	// -작성: 노현기 -일시: 2025.11.10
	bShowMouseCursor = false;


	//bShowMouseCursor = false;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingRef(TEXT("/Game/Character/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingRef.Succeeded())
	{
		InputMappingContext = DefaultMappingRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Character/Input/Action/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Character/Input/Action/IA_Look.IA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SuicideActionRef(TEXT("/Game/Character/Input/Action/IA_Suicide.IA_Suicide"));
	if (SuicideActionRef.Succeeded())
	{
		SuicideAction = SuicideActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Character/Input/Action/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/Character/Input/Action/IA_Attack.IA_Attack"));
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}

	//작성- 한승헌 일시- 2025.11.12
	//InteractionAction 지정
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionRef(TEXT("/Game/Character/Input/Action/IA_Interaction.IA_Interaction"));
	if (InteractionActionRef.Succeeded())
	{
		InteractionAction = InteractionActionRef.Object;
	}

	//작성- 노현기 일시- 2025.11.14
	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionRef(TEXT("/Game/Character/Input/Action/IA_Inventory.IA_Inventory"));
	if (InventoryActionRef.Succeeded())
	{
		InventoryAction = InventoryActionRef.Object;
	}

	//작성- 노현기 일시- 2025.11.14
	static ConstructorHelpers::FObjectFinder<UInputAction> DrawKatanaActionRef(TEXT("/Game/Character/Input/Action/IA_DrawKatana.IA_DrawKatana"));
	if (DrawKatanaActionRef.Succeeded())
	{
		DrawKatanaAction = DrawKatanaActionRef.Object;
	}

	//작성- 노현기 일시- 2025.11.18
	static ConstructorHelpers::FObjectFinder<UInputAction> EquipmentInvenActionRef(TEXT("/Game/Character/Input/Action/IA_EquipmentInven.IA_EquipmentInven"));
	if (EquipmentInvenActionRef.Succeeded())
	{
		EquipmentInvenAction = EquipmentInvenActionRef.Object;
	}

	//HUD 생성 -작성: 한승헌 -일시: 2025.11.07
	static ConstructorHelpers::FClassFinder<UP4HUDWidget> P4HUDWidgetRef(TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));

	if (P4HUDWidgetRef.Succeeded() == true)
	{
		P4HUDWidgetClass = P4HUDWidgetRef.Class;
	}
}

void AP4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Change input mode
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// todo: need to change for other menus such as Pause, Inventory etc.
	//FInputModeUIOnly
	//FInputModeGameAndUI


	// Enhanced Input Subsystem 초기화
	if (InputMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				InputSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	ConsoleCommand(TEXT("showdebug abilitysystem"));


	//HUD 생성 -작성: 한승헌 -일시: 2025.11.07
	P4HUDWidget = CreateWidget<UP4HUDWidget>(this, P4HUDWidgetClass);

	if (P4HUDWidget != nullptr)
	{
		P4HUDWidget->AddToViewport();

		if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
		{
			if (UAbilitySystemComponent* ASC = CharacterPlayer->GetAbilitySystemComponent())
			{
				if (UP4HpBarWidget* HpBar = P4HUDWidget->GetHpBar())
				{
					HpBar->SetAbilitySystemComponent(CharacterPlayer);
				}
			}
		}
	}
	
}
//

void AP4PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(InPawn))
	{
		if (UAbilitySystemComponent* ASC = CharacterPlayer->GetAbilitySystemComponent())
		{
			// 캐릭터의 AbilityMap 읽기
			//for (const auto& Pair : Player->StartInputAbilities)
			//{
			//	const int32 InputID = Pair.Key;
			//	const TSubclassOf<UGameplayAbility> AbilityClass = Pair.Value;

			//	if (AbilityClass)
			//	{
			//		FGameplayAbilitySpec Spec(AbilityClass, 1, InputID, this);
			//		ASC->GiveAbility(Spec);
			//	}
			//}

			// 블루프린트에서 설정해준 초기 어빌리티 부여(인풋X)
			for (const auto& StartAbility : CharacterPlayer->StartAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartAbility);
				ASC->GiveAbility(StartSpec);
			}

			// 블루프린트에서 설정해준 초기 어빌리티 부여(인풋O)
			for (const auto& StartInputAbility : CharacterPlayer->StartInputAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
				StartSpec.InputID = StartInputAbility.Key;
				ASC->GiveAbility(StartSpec);
			}

			SetupGASInputBindings(ASC);

			if (P4HUDWidget != nullptr && P4HUDWidget->GetHpBar() != nullptr)
			{
				P4HUDWidget->GetHpBar()->SetAbilitySystemComponent(CharacterPlayer);
			}
		}
	}

	// -작성: 노현기 -일시: 2025.11.10
	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(InPawn))
	{
		// 인벤토리 컴포넌트 체크
		if (!CharacterPlayer->GetInventoryComponent())
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryComp가 nullptr!"));
			return;
		}

		// 위젯이 아직 생성되지 않았으면 생성
		if (!InventoryWidget)
		{
			if (!InventoryWidgetClass)
			{
				UE_LOG(LogTemp, Error, TEXT("InventoryWidgetClass가 설정되지 않음!"));
				return;
			}

			InventoryWidget = CreateWidget<UP4InventoryWidget>(this, InventoryWidgetClass);
			if (!InventoryWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("인벤토리 위젯 생성 실패!"));
				return;
			}

			InventoryWidget->AddToViewport(UI_TOP_ZORDER);
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		}

		// 인벤토리 바인딩
		InventoryWidget->BindInventory(CharacterPlayer->GetInventoryComponent());

		// -작성: 노현기 -일시: 2025.11.18
		//  장비창 위젯 생성 추가
		if (!EquipmentInvenWidget)
		{
			if (!EquipmentInvenWidgetClass)
			{
				UE_LOG(LogTemp, Error, TEXT("EquipmentInvenWidgetClass가 설정되지 않음!"));
				return;
			}

			EquipmentInvenWidget = CreateWidget<UP4EquipmentInvenWidget>(this, EquipmentInvenWidgetClass);
			if (!EquipmentInvenWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("장비창 위젯 생성 실패!"));
				return;
			}

			EquipmentInvenWidget->AddToViewport(UI_BASE_ZORDER);
			EquipmentInvenWidget->SetVisibility(ESlateVisibility::Hidden);

			//// 인벤토리 위젯 참조 설정
			//if (InventoryWidget)
			//{
			//	EquipmentInvenWidget->SetInventoryWidget(InventoryWidget);
			//}
		}

		// 장비창 바인딩
		if (UP4EquipmentInvenComponent* EquipInvenComp =
			CharacterPlayer->FindComponentByClass<UP4EquipmentInvenComponent>())
		{
			EquipmentInvenWidget->BindEquipInven(EquipInvenComp);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EquipmentInvenComponent를 찾을 수 없음!"));
		}
	}
}

void AP4PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP4PlayerController::HandleMove);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP4PlayerController::HandleLook);
		EIC->BindAction(SuicideAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleSuicide);
		//EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleJumpStart);
		//EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AP4PlayerController::HandleJumpEnd);

		//EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleAttack);
		//SetupPlayerGAS();

		// -작성: 노현기 -일시: 2025.11.14
		EIC->BindAction(InventoryAction, ETriggerEvent::Started, this, &AP4PlayerController::ToggleInventory);

		// -작성: 노현기 -일시: 2025.11.17
		EIC->BindAction(DrawKatanaAction, ETriggerEvent::Started, this, &AP4PlayerController::ToggleDrawSheath);

		// -작성: 노현기 -일시: 2025.11.18
		EIC->BindAction(EquipmentInvenAction, ETriggerEvent::Started, this, &AP4PlayerController::ToggleEquipmentInven);
	}
}

void AP4PlayerController::SetupGASInputBindings(UAbilitySystemComponent* ASC)
{
	if (!ASC || !InputComponent) return;

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// ※참고: - 작성: 노현기 -일시: 2025.11.17 (뒤에 숫자는 나중에 enum class로 만들어서 객관적으로 알 수 있도록 하는게 좋을듯)
		// ( 수정했음 그냥 될 줄 알았는데 (int)형으로 강제 캐스팅 안하면 오류 생기더라구요)
		// Ability Input (InputID Jump = 0, Attack = 1) 
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AP4PlayerController::HandleAbilityPressed, (int)GASInputID::E_JumpAction);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AP4PlayerController::HandleAbilityReleased, (int)GASInputID::E_JumpAction);
		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AP4PlayerController::HandleAbilityPressed, (int)GASInputID::E_AttackAction);

		//작성: 한승헌
		//일시: 2025.11.12
		//NPC와 상호작용을 위한 입력 키.
		EIC->BindAction(InteractionAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleAbilityPressed, (int)GASInputID::E_InteractionAction);

		// -작성: 노현기 -일시: 2025.11.17
		//// 발도 = 3
		//EIC->BindAction(DrawKatanaAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleAbilityPressed, (int)GASInputID::E_DrawKatanaAction);
		//// 납도 = 4
		//EIC->BindAction(DrawKatanaAction, ETriggerEvent::Started, this, &AP4PlayerController::HandleAbilityPressed, (int)GASInputID::E_SheathKatanaAction);
	}
}

void AP4PlayerController::HandleAbilityPressed(int32 InputID)
{
	//// -작성: 노현기 -일시: 2025.11.12
 //   // 공격 입력(1번)이고, 인벤토리가 열려있으면
	//if (InputID == (int)GASInputID::E_AttackAction && bIsInventoryVisible && InventoryWidget)
	//{
	//	// 인벤토리 위젯에게 마우스가 위에 있는지 물어봄
	//	// 있으면 공격 차단
	//	if (InventoryWidget->IsMouseOverInventory())
	//	{
	//		return; // 공격 차단
	//	}
	//}

	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
	{
		CharacterPlayer->GASInputPressed(InputID);
	}
		
}

void AP4PlayerController::HandleAbilityReleased(int32 InputID)
{
	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
	{
		CharacterPlayer->GASInputReleased(InputID);
	}
}

void AP4PlayerController::HandleMove(const FInputActionValue& Value)
{
	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
	{
		CharacterPlayer->HandleMove(Value);
	}
}

void AP4PlayerController::HandleLook(const FInputActionValue& Value)
{
	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
	{
		CharacterPlayer->HandleLook(Value);
	}
}

void AP4PlayerController::HandleSuicide(const FInputActionValue& Value)
{
	if (AP4CharacterPlayer* CharacterPlayer = Cast<AP4CharacterPlayer>(GetPawn()))
	{
		CharacterPlayer->HandleSuicide(Value);
	}
}

// -작성: 노현기 -일시: 2025.11.17
void AP4PlayerController::ToggleDrawSheath()
{
	AP4CharacterPlayer* P4Player = Cast<AP4CharacterPlayer>(GetPawn());
	if (!P4Player)
	{
		UE_LOG(LogTemp, Error, TEXT("[Controller] P4Character를 찾을 수 없습니다!"));
		return;
	}

	// ASC 가져오기
	UAbilitySystemComponent* ASC = P4Player->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("[Controller] ASC를 찾을 수 없습니다!"));
		return;
	}

	FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
	bool bIsDrawn = ASC->HasMatchingGameplayTag(DrawnTag);

	if (bIsDrawn)
	{
		// 손에 쥐고 있음 -> 납도
		UE_LOG(LogTemp, Warning, TEXT("납도 어빌리티 발동 시도"));
		HandleAbilityPressed((int)GASInputID::E_SheathKatanaAction);
	}
	else
	{
		// 등에 있음 -> 발도
		UE_LOG(LogTemp, Warning, TEXT("발도 어빌리티 발동 시도"));
		HandleAbilityPressed((int)GASInputID::E_DrawKatanaAction);
	}

	//USkeletalMeshComponent* MeshComp = P4Player->GetMesh();
	//UP4PlayerAnimInstance* AnimInst = MeshComp ? Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance()) : nullptr;

	//if (!AnimInst)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("[Controller] AnimInstance를 찾을 수 없습니다!"));
	//	return;
	//}

	//// 현재 상태에 따라 발도(3) 또는 납도(4) 어빌리티 발동
	//if (AnimInst->bIsKatanaOnHand)
	//{
	//	// 손에 쥐고 있음 -> 납도 (InputID: 4)
	//	UE_LOG(LogTemp, Warning, TEXT(" 납도 어빌리티 발동 시도"));
	//	HandleAbilityPressed((int)GASInputID::E_SheathKatanaAction);
	//}
	//else
	//{
	//	// 등에 있음 -> 발도 (InputID: 3)
	//	UE_LOG(LogTemp, Warning, TEXT(" 발도 어빌리티 발동 시도"));
	//	HandleAbilityPressed((int)GASInputID::E_DrawKatanaAction);
	//}
}

// -작성: 노현기 -일시: 2025.11.18
void AP4PlayerController::UpdateInputMode()
{
	// UI가 하나라도 열려있는지 확인
	bool bAnyUIVisible = bIsInventoryVisible || bIsEquipmentInvenVisible;

	if (bAnyUIVisible)
	{
		// UI 모드
		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);

		UE_LOG(LogTemp, Log, TEXT("UI 모드 활성화 (인벤토리: %d, 장비창: %d)"),
			bIsInventoryVisible, bIsEquipmentInvenVisible);
	}
	else
	{
		// 게임 전용 모드
		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		UE_LOG(LogTemp, Log, TEXT("게임 모드 활성화"));
	}
}

//작성 - 한승헌 2025-11-10
//임시
void AP4PlayerController::DebugDamage(float Amount)
{
	if (auto* CP = Cast<AP4CharacterPlayer>(GetPawn()))
		if (auto* ASC = CP->GetAbilitySystemComponent())
		{
			// Health에서 Amount만큼 감소
			ASC->ApplyModToAttribute(UP4PlayerAttributeSet::GetHealthAttribute(),
				EGameplayModOp::Additive, -FMath::Abs(Amount));
			UE_LOG(LogTemp, Log, TEXT("[DebugDamage] HP -%0.1f"), Amount);
		}
}

// -작성: 노현기 -일시: 2025.11.10
void AP4PlayerController::ToggleInventory()
{

	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget이 nullptr!"));
		return;
	}

	// 상태 토글
	bIsInventoryVisible = !bIsInventoryVisible;

	if (bIsInventoryVisible)
	{
		// 인벤토리 열기
		//InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		// 인벤토리 닫기
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// UI가 하나라도 열려있으면 UI 모드, 모두 닫혀있으면 게임 모드
	UpdateInputMode();
}

// -작성: 노현기 -일시: 2025.11.18
void AP4PlayerController::ToggleEquipmentInven()
{

	if (!EquipmentInvenWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("EquipmentInvenWidget이 nullptr!"));
		return;
	}

	// 상태 토글
	bIsEquipmentInvenVisible = !bIsEquipmentInvenVisible;

	if (bIsEquipmentInvenVisible)
	{
		// 장비창 열기
		//EquipmentInvenWidget->SetVisibility(ESlateVisibility::Visible);
		EquipmentInvenWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		// 장비창 닫기
		EquipmentInvenWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// UI가 하나라도 열려있으면 UI 모드, 모두 닫혀있으면 게임 모드
	UpdateInputMode();
}

void AP4PlayerController::BringUIToFront(UUserWidget* Widget)
{
	if (!Widget) return;

	// 등록된 위젯들을 직접 참조해서 처리
	TArray<UUserWidget*> AllUIWidgets;

	if (InventoryWidget)
	{
		AllUIWidgets.Add(InventoryWidget);
	}

	if (EquipmentInvenWidget)
	{
		AllUIWidgets.Add(EquipmentInvenWidget);
	}

	// 클릭한 위젯을 제외한 나머지를 아래로
	for (UUserWidget* UI : AllUIWidgets)
	{
		if (UI && UI != Widget && UI->GetVisibility() != ESlateVisibility::Hidden)
		{
			UI->RemoveFromParent();
			UI->AddToViewport(UI_BASE_ZORDER);
		}
	}

	// 클릭한 위젯을 위로
	Widget->RemoveFromParent();
	Widget->AddToViewport(UI_TOP_ZORDER);  // 위 ZOrder

	UE_LOG(LogTemp, Log, TEXT("UI를 최상위로 가져옴"));
}