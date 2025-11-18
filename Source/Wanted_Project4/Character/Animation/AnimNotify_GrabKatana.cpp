// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_GrabKatana.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Inventory/P4InventoryComponent.h"
#include "Item/Equipment/P4WeaponComponent.h"
#include "Character/P4CharacterPlayer.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

UAnimNotify_GrabKatana::UAnimNotify_GrabKatana()
{
}

void UAnimNotify_GrabKatana::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    // ⭐ 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("=== GrabKatana 노티파이 실행! ==="));

    if (!MeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshComp가 nullptr!"));
        return;
    }

    AP4CharacterPlayer* Character = Cast<AP4CharacterPlayer>(MeshComp->GetOwner());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("GrabKatana: 캐릭터를 찾을 수 없습니다!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("캐릭터 찾음: %s"), *Character->GetName());

    // WeaponComponent로 무기를 손으로 이동
    UP4WeaponComponent* WeaponComp = Character->GetWeaponComponent();
    if (!WeaponComp)
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponComponent가 nullptr!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("WeaponComponent 찾음"));

    // 무기를 손으로 이동
    WeaponComp->GrabWeapon();

    //// 애님 인스턴스의 bIsKatanaOnHand를 true로 설정
    //UP4PlayerAnimInstance* AnimInst = Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance());
    //if (AnimInst)
    //{
    //    AnimInst->bIsKatanaOnHand = true;
    //    UE_LOG(LogTemp, Log, TEXT("GrabKatana 노티파이: bIsKatanaOnHand = true"));
    //}

    //UE_LOG(LogTemp, Warning, TEXT("=== GrabKatana 노티파이 완료! ==="));

     // ASC 가져오기
    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Error, TEXT("ASC를 찾을 수 없습니다!"));
        return;
    }

    if (ASC)
    {
        FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
        ASC->AddLooseGameplayTag(DrawnTag);
        UE_LOG(LogTemp, Log, TEXT("Character.State.IsDrawn 태그 추가"));
    }
}
