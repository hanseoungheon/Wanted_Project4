// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_GASDamagedEnd.h"
#include "Character/P4CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotify_GASDamagedEnd::UAnimNotify_GASDamagedEnd()
{
}

FString UAnimNotify_GASDamagedEnd::GetNotifyName_Implementation() const
{
	return TEXT("GASDamagedEnd");
}

void UAnimNotify_GASDamagedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	/*if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			AP4CharacterBase* Character = Cast<AP4CharacterBase>(OwnerActor);
			if (Character)
			{
				Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}

		}
	}*/

    if (MeshComp)
    {
        AActor* OwnerActor = MeshComp->GetOwner();
        if (OwnerActor)
        {
            AP4CharacterBase* Character = Cast<AP4CharacterBase>(OwnerActor);
            if (Character)
            {
                UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
                if (ASC)
                {
                    // 피격 태그 제거
                    Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDAMAGED);

                    // 안전장치: 모든 행동 태그 강제 제거
                    ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking")));

                    UE_LOG(LogTemp, Warning, TEXT("[DamagedEnd] 모든 행동 태그 정리 완료"));
                }

                // 이동 복구
                Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }
        }
    }
}
