// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "P4CustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UP4CustomWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningPawn(APawn* NewOwner) { OwningPawn = NewOwner; }

protected:
	// 현재 위젯을 소유하고 있는 액터 저장용 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
	TObjectPtr<APawn> OwningPawn;

	//작성 - 한승헌
public:
	
	virtual void SetAbilitySystemComponent(AActor* InOwner);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;
};
