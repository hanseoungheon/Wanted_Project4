// Fill out your copyright notice in the Description page of Project Settings.


#include "P4MonsterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AI/P4MonsterAIController.h"
//#include "Attribute/P4PlayerAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/P4Collision.h"
#include "Stat/P4MonsterAttributeSet.h"
#include "Stat/P4MonsterStatComponent.h"
//#include "GameplayTagContainer.h"	HandleGameplayCue()
//#include "GameplayEffectTypes.h"	HandleGameplayCue()
#include "Game/P4GameInstance.h"
#include "Inventory/P4InventoryComponent.h"
#include "Item/ItemDataBase.h"   

class UP4PlayerAttributeSet;
// Sets default values
AP4MonsterBase::AP4MonsterBase()
{
	// ASC 설정===========================================
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UP4MonsterAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UP4MonsterAttributeSet>(AttributeSet);

	//GetCapsuleComponent()->SetCollisionProfileName(CPROPILE_P4CAPSULE);
	
	// Monster Stat Data Table
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Game/Monster/Data/MonsterData.MonsterData")
	);
	if (DataTableRef.Succeeded())
	{
		MonsterStatData = DataTableRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(
		TEXT("/Game/Monster/AI/BB_Monster.BB_Monster")
	);
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Game/Monster/AI/BT_Monster.BT_Monster")
	);
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
	
	// AI 설정===========================================
	AIControllerClass = AP4MonsterAIController::StaticClass();

	// 맵에 배치 또는 생성 시 MonsterAIController 에 빙의되도록
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AP4MonsterBase::BeginPlay()
{
	Super::BeginPlay();
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

	// 공격 델리게이트 배열 설정
	SetupAttackDelegate();
}

void AP4MonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 몬스터 사망 시 발행될 델리게이트 함수 세팅
	AttributeSet->OnHpZero.AddUObject(this, &AP4MonsterBase::SetDead);
}

void AP4MonsterBase::AttackHitCheck()
{
	// @Todo: 공격 판정 구현 필요(델리게이트로 바꾸면서 미사용)
}

//void AP4MonsterBase::HandleGameplayCue(AActor* MyTarget, FGameplayTag EventTag, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
//{
//	Super::HandleGameplayCue(MyTarget, EventTag, EventType, Parameters);
//	
//	if (AttributeSet->GetCurHP()>0 && EventTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Monster.Hit"))))
//	{
//		HitActionBegin(); // 넉백 / 피격 몽타주 실행
//		UE_LOG(LogTemp, Log, TEXT("HitActionBegin"));
//	}
//}

void AP4MonsterBase::ApplyDamage(const float DamageAmount)
{
	if (ASC)
	{
		// Damaged 몽타주 실행
		DamagedActionBegin();

		// 체력 감소 적용 부분
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		Context.AddSourceObject(this);

		// 받을 데미지 설정
		AttributeSet->SetDamageAmount(DamageAmount);

		// 블루프린트로 생성한 GameplayEffect 클래스 불러오기
		FSoftClassPath GEPath(TEXT("/Game/Monster/GE/BPGE_MonsterDamaged.BPGE_MonsterDamaged_C"));
		TSoftClassPtr<UGameplayEffect> DamagedEffectSoftClass(GEPath);

		// 메모리에 아직 BPGE_MonsterDamaged 가 없으면
		if (DamagedEffectSoftClass.IsPending())
		{
			// 동기 로드
			DamagedEffectSoftClass.LoadSynchronous();
		}

		// 로드 성공 시 사용
		TSubclassOf<UGameplayEffect> DamagedEffectClass = DamagedEffectSoftClass.Get();
		if (DamagedEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle
				= ASC->MakeOutgoingSpec(DamagedEffectClass, 1.f, Context);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				UE_LOG(LogTemp, Log, TEXT("몬스터 데미지 호출"));
				UE_LOG(LogTemp, Log, TEXT("CurHP: %f"), AttributeSet->GetCurHP());
			}
		}
	}
}

void AP4MonsterBase::GiveDamage(AActor* TargetActor, const float DamageAmount)
{
	// todo: Player만 데미지 전달하는걸로 바꾸기
	if (IP4DamageableInterface* Character = Cast<IP4DamageableInterface>(TargetActor))
	{
		Character->ApplyDamage(DamageAmount); // 공격자 정보도 전달
	}

	//UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	//if (!TargetASC)
	//{
	//	return;
	//}

	//UP4PlayerAttributeSet* TargetAttribute = const_cast<UP4PlayerAttributeSet*>(TargetASC->GetSet<UP4PlayerAttributeSet>());
	//if (!TargetAttribute)
	//{
	//	return;
	//}
	//
	//// 일단 Player 의 Attribute에 직접 접근하여 감소 시킴
	//TargetAttribute->SetHealth(TargetAttribute->GetHealth() - DamageAmount);
}

void AP4MonsterBase::AttackByAI()
{
	// 각 몬스터 Class 에서 AttackActionBegin(FName& InAttackMontageSectionName) 으로
	// 공격 몽타주 섹션 이름 넘겨줘서 공격 실행
}

void AP4MonsterBase::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	// 전달받은 델리게이트 저장
	OnAttackFinished = InOnAttackFinished;
}

void AP4MonsterBase::NotifyActionEnd()
{
	// 공격 끝날 시 전달받은 델리게이트 호출
	// 여기서는 공격 완료 상태 전달
	OnAttackFinished.ExecuteIfBound();
}

void AP4MonsterBase::AttackActionBegin(FName& InAttackMontageSectionName, const float AttackSpeed)
{
	// 공격 모션동안 이동 막기
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	// 몽타주 재생을 위해 AnimInstance 갖고 오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		IsAttacking = true;
		
		// 입력받은 섹션으로 몽타주 섹션 변경
		AnimInstance->Montage_JumpToSection(InAttackMontageSectionName, AttackActionMontage);

		// 몽타주 실행 
		AnimInstance->Montage_Play(AttackActionMontage, AttackSpeed);

		// 몽타주 재생이 끝날 때 실행될 함수 바인딩
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AP4MonsterBase::AttackActionEnd);

		// 몽타주 재생 종료 시 바인딩한 델리게이트 실행
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, AttackActionMontage);
	}
}

void AP4MonsterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	IsAttacking = false;
	
	// 피격 모션이 진행중이 아니라면
	if (IsDamaged == false)
	{
	    // 무브먼트 모드 복구
	    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	// 공격이 끝났음을 알림
	NotifyActionEnd();
}

void AP4MonsterBase::DamagedActionBegin()
{
	// Damaged 몽타주 실행
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		IsDamaged = true;
				
		// @Todo: 임시 Damaged 모션 시작 로그
		UE_LOG(LogTemp, Log, TEXT("[Monster] Monster Damaged Action Begin"));
		
		// Damaged 모션동안 이동 막기
		GetCharacterMovement()->SetMovementMode(MOVE_None);

		// Damaged 몽타주 재생
		AnimInstance->Montage_Play(DamagedMontage, 1.f);

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(
			this, &AP4MonsterBase::DamagedActionEnd
		);

		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, DamagedMontage);
	}
}

void AP4MonsterBase::DamagedActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{	
	// @Todo: 임시 Damaged 모션 끝 로그
	UE_LOG(LogTemp, Log, TEXT("[Monster] Monster Damaged Action End"));

	IsDamaged = false;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AP4MonsterBase::SetDead()
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

	// -작성: 노현기 -일시: 2025.11.19
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

// -작성: 노현기 -일시: 2025.11.19
void AP4MonsterBase::DropRandomItems()
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
		const FMonsterDropItem& DropInfo = DropItemPool[ItemIndices[i]];

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

void AP4MonsterBase::SetupAttackDelegate()
{
}

void AP4MonsterBase::ExecuteAttackSection(const FName& SectionName)
{
	// 섹션 이름을 Index로 변경
	int32 Index = AttackSectionNames.IndexOfByKey(SectionName);

	// @MobTODO: 몽타주 노티파이 확인용 로그
	UE_LOG(LogTemp, Log, TEXT("몽타주 재생, Index : %d"), Index);
	
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

UAbilitySystemComponent* AP4MonsterBase::GetAbilitySystemComponent() const
{
	return ASC;
}
