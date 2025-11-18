// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Animation/AnimNotifyState_MonsterDashCheck.h"

#include "Character/P4CharacterBase.h"
#include "Engine/OverlapResult.h"
#include "Monster/Mob/P4BossMonsterNemielle.h"

void UAnimNotifyState_MonsterDashCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	IsPlayerDamaged = false;
	
}

void UAnimNotifyState_MonsterDashCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	// 플레이어가 이미 데미지를 받았으면 스킵
	if (IsPlayerDamaged)
	{
		return;
	}
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AP4BossMonsterBase* BossMonster = Cast<AP4BossMonsterBase>(MeshComp->GetOwner());
	if (BossMonster)
	{
		// 패턴 데이터 불러오기
		FPatternData Pattern = BossMonster->GetPatternComponent()->GetPatterns("DashAttack");

		const float AttackRange = Pattern.AttackRange;

		FVector Start =
			BossMonster->GetActorLocation() +
			BossMonster->GetActorForwardVector() *
			AttackRange;
		
		const float AttackRadius = Pattern.AttackRadius;

		// 자신은 판정 제외
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, BossMonster);

		TArray<FOverlapResult> OutHitResults;
		bool HitDetected = MeshComp->GetWorld()->OverlapMultiByChannel(
			OutHitResults,
			Start,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			FCollisionShape::MakeBox(FVector(AttackRadius, AttackRadius, AttackRadius)),
			Params
		);

		// 몬스터 공격 범위 디버그 표시
#if ENABLE_DRAW_DEBUG
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugBox(
			MeshComp->GetWorld(),
			Start,
			FVector(AttackRadius, AttackRadius, AttackRadius),
			FRotationMatrix::MakeFromX(BossMonster->GetActorForwardVector()).ToQuat(),
			DrawColor,
			false,
			5.f
		);
#endif
		if (HitDetected)
		{
			for (auto& OutHitResult : OutHitResults)
			{
				AP4CharacterBase* Player = Cast<AP4CharacterBase>(OutHitResult.GetActor());
				if (Player)
				{
					// 중복 충돌 방지
					IsPlayerDamaged = true;
					
					// @MobTODO: 몬스터 충돌 판정 확인용
					UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트: %s"), *OutHitResult.GetActor()->GetName());

					// 다른 액터가 공격 당했을 시 처리
					BossMonster->GiveDamage(OutHitResult.GetActor(), BossMonster->GetAttributeSet()->GetAttack());
				}
			}
		}
		else
		{
			// @MobTODO: 몬스터 충돌 판정 확인용
			//UE_LOG(LogTemp, Log, TEXT("몬스터 공격 시 충돌된 오브젝트가 없습니다."));
		}
	}
}

void UAnimNotifyState_MonsterDashCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	
}
