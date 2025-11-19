// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_ReleaseKatana.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Inventory/P4InventoryComponent.h"
#include "Item/Equipment/P4WeaponComponent.h"
#include "Character/P4CharacterPlayer.h"
#include "Character/Animation/P4PlayerAnimInstance.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Tag/P4GameplayTag.h"

UAnimNotify_ReleaseKatana::UAnimNotify_ReleaseKatana()
{
}

void UAnimNotify_ReleaseKatana::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    // ⭐ 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("=== ReleaseKatana 노티파이 실행! ==="));

    if (!MeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshComp가 nullptr!"));
        return;
    }

    AP4CharacterPlayer* Character = Cast<AP4CharacterPlayer>(MeshComp->GetOwner());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReleaseKatana: 캐릭터를 찾을 수 없습니다!"));
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

    //  무기를 손으로 이동
    WeaponComp->SheathWeapon();

    // ASC 가져오기
    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Error, TEXT("ASC를 찾을 수 없습니다!"));
        return;
    }

    // 태그 제거
    //FGameplayTag DrawnTag = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDrawn"));
    ASC->RemoveLooseGameplayTag(P4TAG_CHARACTER_ISDRAWN);
    UE_LOG(LogTemp, Log, TEXT("Character.State.IsDrawn 태그 제거"));

    //// ⭐ 애님 인스턴스의 bIsKatanaOnHand를 false로 설정
    //UP4PlayerAnimInstance* AnimInst = Cast<UP4PlayerAnimInstance>(MeshComp->GetAnimInstance());
    //if (AnimInst)
    //{
    //    AnimInst->bIsKatanaOnHand = false;
    //    UE_LOG(LogTemp, Log, TEXT("ReleaseKatana 노티파이: bIsKatanaOnHand = false"));
    //}

    //UE_LOG(LogTemp, Warning, TEXT("=== ReleaseKatana 노티파이 완료! ==="));
}
