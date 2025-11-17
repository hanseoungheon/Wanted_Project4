// Fill out your copyright notice in the Description page of Project Settings.


#include "P4GA_Interaction.h"
#include "UI/P4QuestWidget.h"
#include "Character/P4CharacterPlayer.h"
#include "NPC/P4NPCBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Game/P4GameInstance.h"
#include "Player/P4PlayerController.h"
UP4GA_Interaction::UP4GA_Interaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP4GA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 로컬 플레이어에서만 UI 생성
	if (ActorInfo->IsLocallyControlled() == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	//캐릭터 가져오기.
	AP4CharacterPlayer* PlayerCharacter
		= Cast<AP4CharacterPlayer>(ActorInfo->AvatarActor.Get());

	//태그를 찾기 위한 ASC 컴포넌트
	UAbilitySystemComponent* ASC 
		= ActorInfo->AbilitySystemComponent.Get();

	if (ASC == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}


	AP4PlayerController* PC = Cast<AP4PlayerController>(PlayerCharacter->GetController());

	PC->Test();
	// 1) 루트 인터랙트 태그부터 검사 (Character.Interact)
	//static const FGameplayTag InteractTag =
	//	FGameplayTag::RequestGameplayTag(TEXT("Character.Interact"));

	//if (ASC->HasMatchingGameplayTag(InteractTag) == false)
	//{
	//	// 범위 밖이거나, 상호작용 대상이 없는 상태 → 그냥 조용히 종료
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//	return;
	//}

	if (PlayerCharacter != nullptr)
	{

		//NPC 가져오기.
		AP4NPCBase* NPC_Character 
			= Cast<AP4NPCBase>(PlayerCharacter->CurrentInteractActor);

		if (NPC_Character == nullptr)
		{	
			UE_LOG(LogTemp, Display, TEXT("NPCCharacter NULLPTR"));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}


		if (
			ASC
				->HasMatchingGameplayTag(
					FGameplayTag::RequestGameplayTag(TEXT("Character.Interaction.Quest"))
				)
			) //퀘스트 태그를 가지고 있는지?
		{
			//퀘스트 매니저 가져오기.
			UWorld* world = GetWorld();
			UP4GameInstance* GI = world->GetGameInstance<UP4GameInstance>();

			UP4QuestManager* QuestManager 
				= ((GI != nullptr) ? GI->QuestManager : nullptr); //GI가 있으면 퀘스트 매니저 가져오고 아니면 nullptr할당.


			if (QuestManager == nullptr) 
				return;

			if (QuestManager->IsQuestActive() == false)
			{
				NPC_Character->ShowQuestUI(NPC_Character->GetNPCQuestCode());
			}
			else
			{
				if (NPC_Character->GetReportObjectiveID().IsEmpty() == false) //퀘스트를 이미 진행중이면.
				{
					QuestManager->UpdateObjective(NPC_Character->GetReportObjectiveID());
				}
			}
		}
		else if (
			ASC
			->HasMatchingGameplayTag(
				FGameplayTag::RequestGameplayTag(TEXT("Character.Interaction.Enchant"))
			)
			) //인첸트(무기강화) 태그를 가지고 있는지?
		{
			//Todo: 무기강화 구현해야됨.
			NPC_Character->ShowEnchantUI();
		}

		//NPC_Character->ShowQuestUI(NPC_Character->GetNPCQuestCode());
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UP4GA_Interaction::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//필요하면 구현.
}

void UP4GA_Interaction::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	//필요하면 구현.
}

void UP4GA_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,bWasCancelled);
}
