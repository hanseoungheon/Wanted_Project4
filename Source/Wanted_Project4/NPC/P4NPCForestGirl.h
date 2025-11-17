// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/P4NPCBase.h"
#include "P4NPCForestGirl.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API AP4ForestGirl : public AP4NPCBase
{
	GENERATED_BODY()
	//함수 섹션.
public:
	AP4ForestGirl();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void Interaction(ACharacter* Character) override;

protected:


	//GAS섹션.
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:




	//변수 섹션
public:

protected:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	TObjectPtr<class UAnimMontage> RequsetMontage; //퀘스트 수주시 모션 몽타주
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	TObjectPtr<class UAnimMontage> QusetClearMomtage; //퀘스트 완료시 모션 몽타주



	
};
