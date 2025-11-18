// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4CustomWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

//작성 한승헌
void UP4CustomWidget::SetAbilitySystemComponent(AActor* InOwner)
{
    if (IsValid(InOwner) == true)
    {
        ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
    }
}

//작성 한승헌
UAbilitySystemComponent* UP4CustomWidget::GetAbilitySystemComponent() const
{
    return ASC;
}
