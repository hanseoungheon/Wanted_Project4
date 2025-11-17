// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/P4BossMonsterBase.h"

#include "AI/P4MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stat/P4MonsterStat.h"

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
		// @MobTodo: 몽타주 재생 확인용 로그
		UE_LOG(LogTemp, Log, TEXT("Monster Has AnimInstance"));

		// 입력받은 섹션으로 몽타주 섹션 변경
		AnimInstance->Montage_JumpToSection(InAttackMontageSectionName, AttackActionMontage);

		// 몽타주 실행 
		AnimInstance->Montage_Play(AttackActionMontage, AttackSpeed);

		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			IsPatternActive = true;
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsPatternActive"), true);

			// @MobTodo: 몽타주 재생 확인용 로그
			UE_LOG(LogTemp, Log, TEXT("Monster Has AIController"));
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
