// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Rolling.h"
#include "GameFramework/Character.h"
#include "Interface/P4RollingInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_Rolling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character == nullptr)
	{
		return;
	}

	UWorld* world = MeshComp->GetWorld();

	if (world == nullptr || world->IsGameWorld() == false)
	{
		return;
	}

	//FVector RollDir = Character->GetActorForwardVector();

	IP4RollingInterface* RollInterface = Cast<IP4RollingInterface>(Character);

	if(RollInterface == nullptr)
	{
		return;
	}

	FVector RollDir = RollInterface->GetRollingDirection();
	RollDir.Normalize();
	//FRotator CurrentRot = Character->GetActorRotation();


	Character->SetActorRotation(RollDir.Rotation());

	//Character->AddActorLocalRotation(PlusRot);
	RollInterface->StartRollingTimeLine();
}
