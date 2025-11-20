// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P4WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WANTED_PROJECT4_API UP4WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP4WeaponComponent();

    // 무기 장착/해제
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool EquipWeapon(class UItemDataBase* WeaponData);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void UnequipWeapon();

    // 무기 소켓 전환 (등 ↔ 손)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AttachWeaponToSocket(FName SocketName);

    // AnimNotify에서 호출할 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void GrabWeapon();  // 등 → 손

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SheathWeapon();  // 손 → 등

    //todo: 선우 - Getter 사용하기?
    // 현재 장착된 무기 메시
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;
private:
    void CreateWeaponMesh(USkeletalMesh* Mesh);
    void DestroyWeaponMesh();

protected:

    // 현재 장착된 무기 데이터
    UPROPERTY()
    TObjectPtr<class UItemDataBase> CurrentWeaponData;

    // 소켓 이름들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName BackSocketName = FName("back_socket");  // 등 소켓

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName HandSocketName = FName("hand_RSocket");  // 손 소켓
};
