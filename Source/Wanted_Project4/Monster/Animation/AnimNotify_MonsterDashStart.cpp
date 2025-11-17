// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Animation/AnimNotify_MonsterDashStart.h"

#include "AbilitySystemComponent.h"
#include "Monster/P4BossMonsterBase.h"

void UAnimNotify_MonsterDashStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
		
	APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
	if (Pawn)
	{
		AP4BossMonsterBase* Monster = Cast<AP4BossMonsterBase>(Pawn);
		if (Monster)
		{
			Monster->OnDashStart.Broadcast();
			Monster->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		
		UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC)
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Monster.Action.DashStart"));
		}
	}
}
