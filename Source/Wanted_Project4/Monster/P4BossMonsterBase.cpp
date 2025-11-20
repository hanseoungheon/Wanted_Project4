// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/P4BossMonsterBase.h"

#include "AI/P4MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stat/P4MonsterStat.h"
#include "Inventory/P4InventoryComponent.h"

// Sets default values
AP4BossMonsterBase::AP4BossMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ASC 설정===========================================
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UP4MonsterAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UP4MonsterAttributeSet>(AttributeSet);


	PatternComponent = CreateDefaultSubobject<UP4MonsterPatternComponent>(TEXT("PatternComponent"));

	// Monster Stat Data Table
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Game/Monster/Data/MonsterData.MonsterData")
	);
	if (DataTableRef.Succeeded())
	{
		MonsterStatData = DataTableRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(
		TEXT("/Game/Monster/AI/BB_BossMonster.BB_BossMonster")
	);
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Game/Monster/AI/BT_BossMonster.BT_BossMonster")
	);
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}

	// AI 설정
	AIControllerClass = AP4MonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AP4BossMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	// Stat 설정
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		// 데이터 테이블에서 초기 스탯을 불러와서 AttributeSet 초기화
		if (AttributeSet)
		{
			// 데이터 테이블에서 이름으로 Row 값 받아와서 값 설정하기
			if (const FP4MonsterStat* Row = MonsterStatData->FindRow<FP4MonsterStat>(
				MonsterID, TEXT("Monster Data Init")))
			{
				// AttributeSet 세팅
				AttributeSet->SetMaxHP(Row->MaxHP);
				AttributeSet->SetCurHP(Row->MaxHP);
				AttributeSet->SetDetectRange(Row->DetectRange);
				AttributeSet->SetChaseRange(Row->ChaseRange);
				AttributeSet->SetMovementSpeed(Row->MovementSpeed);
				AttributeSet->SetChaseSpeed(Row->ChaseSpeed);
				AttributeSet->SetTurnSpeed(Row->TurnSpeed);
				AttributeSet->SetAttack(Row->Attack);
				AttributeSet->SetAttackSpeed(Row->AttackSpeed);
				AttributeSet->SetGroggyGauge(0.f);

				// MonsterBase 의 선공 여부 세팅
				bIsAgressive = Row->bIsAggressive;
			}
		}
	}

	// 일정 시간마다 패턴 확률 체크
	GetWorld()->GetTimerManager().SetTimer(
		PatternCheckTimerHandle,
		this,
		&AP4BossMonsterBase::CheckPatternProbability,
		1.0f,
		true
	);

	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();

	// 공격 델리게이트 배열 설정
	SetupAttackDelegate();
}

void AP4BossMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 몬스터 사망 시 발행될 델리게이트 함수 세팅
	AttributeSet->OnHpZero.AddUObject(this, &AP4BossMonsterBase::SetDead);

	// 몬스터 그로기 게이지 가득 찰 시 발행될 델리게이트 함수 세팅
	AttributeSet->OnGroggyStart.AddUObject(this, &AP4BossMonsterBase::GroggyStart);
}

// Called every frame
void AP4BossMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AP4BossMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AP4BossMonsterBase::GetIsDamaged()
{
	return false;
}

bool AP4BossMonsterBase::GetIsAttacking()
{
	return false;
}

void AP4BossMonsterBase::AttackByAI()
{
}

void AP4BossMonsterBase::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	// 전달받은 델리게이트 저장
	OnAttackFinished = InOnAttackFinished;
}

void AP4BossMonsterBase::NotifyActionEnd()
{
	// 공격 끝날 시 전달받은 델리게이트 호출
	// 여기서는 공격 완료 상태 전달
	OnAttackFinished.ExecuteIfBound();
}

void AP4BossMonsterBase::AttackActionBegin(FName& InAttackMontageSectionName, const float AttackSpeed)
{
	// 공격 모션동안 이동 막기
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	// 몽타주 재생을 위해 AnimInstance 갖고 오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 섹션을 변경하고 몽타주를 실행했더니 맨 처음 섹션만 재생되는 버그 발생
		// Montage_Play 의 경우 재생위치를 0으로 덮어쓰기 때문에 발생하는 거였음
		// 몽타주 실행 후 섹션을 변경하도록 수정
		// 몽타주 실행 
		AnimInstance->Montage_Play(AttackActionMontage, AttackSpeed);

		// 입력받은 섹션으로 몽타주 섹션 변경
		AnimInstance->Montage_JumpToSection(InAttackMontageSectionName, AttackActionMontage);

		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			IsPatternActive = true;
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsPatternActive"), true);
		}

		// 몽타주 재생이 끝날 때 실행될 함수 바인딩
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AP4BossMonsterBase::AttackActionEnd);

		// 몽타주 재생 종료 시 바인딩한 델리게이트 실행
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, AttackActionMontage);
	}
}

void AP4BossMonsterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	// 무브먼트 모드 복구
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		IsPatternActive = false;
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsPatternActive"), false);
	}

	// 공격이 끝났음을 알림
	NotifyActionEnd();
}

void AP4BossMonsterBase::SetDead()
{
	// 이동 못하게 막기
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	// 사망 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(DeadMontage, 3.f);
	}

	// 콜리전 끄기
	SetActorEnableCollision(false);

	// AI 재생 멈추기
	AP4MonsterAIController* P4MonsterAIController = Cast<AP4MonsterAIController>(GetController());
	if (P4MonsterAIController)
	{
		P4MonsterAIController->StopAI();
	}

	// -작성: 노현기 -일시: 2025.11.20
	// 랜덤 아이템 드롭
	DropRandomItems();

	// DeadEventDelayTime 후 액터 삭제
	FTimerHandle DeadTimerHandle;
	float DeadEventDelayTime = 5.f;
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		[&]()
		{
			Destroy();
		},
		DeadEventDelayTime,
		false
	);
}

// -작성: 노현기 -일시: 2025.11.20
void AP4BossMonsterBase::DropRandomItems()
{
	// 드롭 아이템 목록이 비어있으면 리턴
	if (DropItemPool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 드롭 아이템 목록이 비어있습니다."), *this->GetName());
		return;
	}

	// 플레이어의 InventoryComponent 찾기
	APlayerController* PC = this->GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] PlayerController를 찾을 수 없습니다."), *this->GetName());
		return;
	}

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] PlayerPawn을 찾을 수 없습니다."), *this->GetName());
		return;
	}

	UP4InventoryComponent* InvComp = PlayerPawn->FindComponentByClass<UP4InventoryComponent>();
	if (!InvComp)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] InventoryComponent를 찾을 수 없습니다."), *this->GetName());
		return;
	}

	// 드롭할 아이템 종류 개수 결정 (1~4개 사이)
	int32 NumItemTypesToDrop = FMath::RandRange(
		FMath::Min(MinDropItemTypes, DropItemPool.Num()),
		FMath::Min(MaxDropItemTypes, DropItemPool.Num())
	);

	UE_LOG(LogTemp, Log, TEXT("[%s] %d종류의 아이템 드롭 시도"), *this->GetName(), NumItemTypesToDrop);

	// 드롭 풀을 섞기 위한 임시 배열 생성
	TArray<int32> ItemIndices;
	for (int32 i = 0; i < DropItemPool.Num(); ++i)
	{
		ItemIndices.Add(i);
	}

	// 배열 섞기 (Fisher-Yates shuffle)
	for (int32 i = ItemIndices.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::RandRange(0, i);
		ItemIndices.Swap(i, j);
	}

	// 섞인 배열에서 앞에서부터 N개 선택
	int32 SuccessCount = 0;
	for (int32 i = 0; i < NumItemTypesToDrop; ++i)
	{
		const FBossMonsterDropItem& DropInfo = DropItemPool[ItemIndices[i]];

		if (!DropInfo.ItemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] 드롭 아이템[%d]이 nullptr"), *this->GetName(), i);
			continue;
		}

		// 드롭 확률 체크 (0.0 ~ 1.0)
		float RandomChance = FMath::FRand();  // 0.0 ~ 1.0 사이의 랜덤 값
		if (RandomChance > DropInfo.DropChance)
		{
			UE_LOG(LogTemp, Log, TEXT("[%s] %s 드롭 실패 (확률: %.2f, 랜덤: %.2f)"),
				*this->GetName(),
				*DropInfo.ItemData->GetItemName().ToString(),
				DropInfo.DropChance,
				RandomChance);
			continue;
		}

		// 해당 아이템의 개수를 Min~Max 사이에서 랜덤 결정
		int32 DropCount = FMath::RandRange(DropInfo.MinQuantity, DropInfo.MaxQuantity);

		// 인벤토리에 아이템 추가
		bool bSuccess = InvComp->AddItem(DropInfo.ItemData, DropCount);

		if (bSuccess)
		{
			SuccessCount++;
			UE_LOG(LogTemp, Log, TEXT("[%s] 아이템 드롭 성공: %s x%d"),
				*this->GetName(),
				*DropInfo.ItemData->GetItemName().ToString(),
				DropCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] %s 드롭 실패: 인벤토리 공간 부족"),
				*this->GetName(),
				*DropInfo.ItemData->GetItemName().ToString());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[%s] 총 %d종류 아이템 드롭 성공"), *this->GetName(), SuccessCount);
}

void AP4BossMonsterBase::SetupAttackDelegate()
{
}

void AP4BossMonsterBase::ExecuteAttackSection(const FName& SectionName)
{
	// 섹션 이름을 Index로 변경
	int32 Index = AttackSectionNames.IndexOfByKey(SectionName);

	if (AttackDelegates.IsValidIndex(Index) && AttackDelegates[Index].IsBound())
	{
		// 해당 인덱스의 함수 실행
		AttackDelegates[Index].Execute();
	}

	// 함수 없으면 로그 띄움
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s 에 해당하는 함수가 없습니다."), *SectionName.ToString());
	}
}

void AP4BossMonsterBase::CheckPatternProbability()
{
	// 죽은 상태
	if (GetAttributeSet()->GetCurHP() <= 0.f)
	{
		return;
	}

	// 그로기 상태
	if (IsGroggyStatus)
	{
		return;
	}

	// 만약 패턴이 실행 중이라면 반환
	if (IsPatternActive)
	{
		return;
	}

	// 패턴 컴포넌트에서 가중치로 패턴 확인
	AP4MonsterAIController* AICon = Cast<AP4MonsterAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("Target")));
			PatternComponent->TryExecutePattern(Target);
		}
	}
}

void AP4BossMonsterBase::ApplyDamage(const float DamageAmount)
{
	if (ASC)
	{
		// 체력 감소 적용 부분
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		Context.AddSourceObject(this);

		// 받을 데미지 설정
		AttributeSet->SetDamageAmount(DamageAmount);

		// 로드 성공 시 사용
		TSubclassOf<UGameplayEffect> DamagedEffectClass
			= GetEffectClass(TEXT("/Game/Monster/GE/BPGE_MonsterDamaged.BPGE_MonsterDamaged_C"));

		if (DamagedEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle
				= ASC->MakeOutgoingSpec(DamagedEffectClass, 1.f, Context);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

		// PostGameplayEffectExecute 가 실행될려면 GameplayEffect 로 작동
		// Groggy Gauge 부여
		// 그로기 상태가 아닐때만 그로기 게이지 쌓기
		if (IsGroggyStatus == false)
		{
			// 로드 성공 시 사용
			TSubclassOf<UGameplayEffect> GroggyEffectClass
				= GetEffectClass(TEXT("/Game/Monster/GE/BPGE_MonsterGroggy.BPGE_MonsterGroggy_C"));
			if (GroggyEffectClass)
			{
				FGameplayEffectSpecHandle SpecHandle
					= ASC->MakeOutgoingSpec(GroggyEffectClass, 1.f, Context);
				if (SpecHandle.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

void AP4BossMonsterBase::GiveDamage(AActor* TargetActor, const float DamageAmount)
{
	if (IP4DamageableInterface* Character = Cast<IP4DamageableInterface>(TargetActor))
	{
		Character->ApplyDamage(DamageAmount); // 공격자 정보도 전달
	}
}

TSubclassOf<UGameplayEffect> AP4BossMonsterBase::GetEffectClass(FString Path)
{
	// 블루프린트로 생성한 GameplayEffect 클래스 불러오기
	FSoftClassPath GEPath(Path);
	TSoftClassPtr<UGameplayEffect> EffectSoftClass(GEPath);

	// 메모리에 아직 BPGE_MonsterDamaged 가 없으면
	if (EffectSoftClass.IsPending())
	{
		// 동기 로드
		EffectSoftClass.LoadSynchronous();
	}

	return EffectSoftClass.Get();
}

void AP4BossMonsterBase::GroggyStart()
{
	IsGroggyStatus = true;

	// 그로기 모션동안 이동 막기
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	AP4MonsterAIController* AICon = Cast<AP4MonsterAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsBool(TEXT("IsGroggy"), true);
		}
	}

	// 몽타주 재생을 위해 AnimInstance 갖고 오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		IsGroggyLoopEnd = false;
		AnimInstance->StopAllMontages(0.f);

		// 몽타주 실행 
		AnimInstance->Montage_Play(GroggyMontage, 2.f);

		// 일정 시간 후 그로기 종료 시키기 위해 실행될 함수 바인딩
		GetWorld()->GetTimerManager().SetTimer(
			GroggyTimerHandle,
			[this]()
			{
				IsGroggyLoopEnd = true;
			},
			10.0f,
			false
		);
	}
}

void AP4BossMonsterBase::GroggyLoopCheck()
{
	if (IsGroggyLoopEnd == false)
	{
		return;
	}

	GroggyEnd();
}

void AP4BossMonsterBase::GroggyEnd()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_JumpToSection(FName("MonsterGroggyEnd"), GroggyMontage);

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this](UAnimMontage* TargetMontage, bool Interrupted)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			IsGroggyStatus = false;
			AttributeSet->SetGroggyGauge(0.f);
			
			AP4MonsterAIController* AICon = Cast<AP4MonsterAIController>(GetController());
			if (AICon)
			{
				UBlackboardComponent* BB = AICon->GetBlackboardComponent();
				if (BB)
				{
					BB->SetValueAsBool(TEXT("IsGroggy"), false);
				}
			}
		});

		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, GroggyMontage);
	}
}
