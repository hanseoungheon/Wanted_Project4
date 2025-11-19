// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Animation/AnimNotify_GroggyLoopEnd.h"

#include "Monster/P4BossMonsterBase.h"

void UAnimNotify_GroggyLoopEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AP4BossMonsterBase* Boss = Cast<AP4BossMonsterBase>(MeshComp->GetOwner());
	if (Boss)
	{
		Boss->GroggyLoopCheck();
	}
}
