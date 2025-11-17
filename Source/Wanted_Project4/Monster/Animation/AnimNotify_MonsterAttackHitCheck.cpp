// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterAttackHitCheck.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequenceBase.h"
#include "Monster/P4MonsterBase.h"
#include "Wanted_Project4/Interface/AnimationAttackInterface.h"


void UAnimNotify_MonsterAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 인터페이스로 호출이면 이렇게
	// if (MeshComp)
	// {
	// 	// MeshComponent 를 갖고있는 Actor 에 붙어있는 AnimationAttackInterface 불러오기
	// 	IAnimationAttackInterface* AttackPawn
	// 		= Cast<IAnimationAttackInterface>(MeshComp->GetOwner());
	// 	if (AttackPawn)
	// 	{
	// 		// 있다면 AttackHitCheck 호출
	// 		AttackPawn->AttackHitCheck();
	// 	}
	// }

	// 몬스터 몽타주 섹션에 따른 실행
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
		if (Montage)
		{
			FName CurrentSection = AnimInstance->Montage_GetCurrentSection(Montage);
			IMonsterAIInterface* Monster = Cast<IMonsterAIInterface>(MeshComp->GetOwner());
			if (Monster)
			{
				Monster->ExecuteAttackSection(CurrentSection);
			}
		}
	}
}
