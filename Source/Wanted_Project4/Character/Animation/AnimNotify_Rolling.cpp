// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Rolling.h"
#include "GameFramework/Character.h"
#include "Interface/P4RollingInterface.h"
void UAnimNotify_Rolling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

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

	FVector RollDir = Character->GetActorForwardVector();

	if (IP4RollingInterface* RollInterface = Cast<IP4RollingInterface>(Character))
	{
		RollDir = RollInterface->GetRollingDirection();
	}

	RollDir.Normalize();

	Character->LaunchCharacter(RollDir * RollPower, true, false);
	UE_LOG(LogTemp, Warning, TEXT("Rolling Notify! Dir:%s, Power:%f"),
		*RollDir.ToString(), RollPower);
}
