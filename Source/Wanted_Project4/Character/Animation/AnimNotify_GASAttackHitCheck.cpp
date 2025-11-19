// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = DamageMultiplier;
			//PayloadData.EventMagnitude = ComboAttackLevel;

			// ASC가진 액터에 태그 부착(Character.Action.AttackHitCheck) 해서 이 태그가 트리거 태그로 지정된 BPGA_AttackHitCheck 실행.
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
