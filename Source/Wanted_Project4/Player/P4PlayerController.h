// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "P4PlayerController.generated.h"

UENUM(BlueprintType)
enum class GASInputID : uint8
{
    E_JumpAction = 0,
    E_AttackAction,
    E_InteractionAction,
    E_DrawKatanaAction,
    E_SheathKatanaAction,

};

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;

UCLASS()
class WANTED_PROJECT4_API AP4PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AP4PlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    // 입력 바인딩
    virtual void SetupInputComponent() override;
    // GAS 입력 바인딩
    void SetupGASInputBindings(UAbilitySystemComponent* ASC);

    // -작성: 노현기 -일시: 2025.11.10
    // 인벤토리 토글 함수
    void ToggleInventory();
   
    // -작성: 노현기 -일시: 2025.11.14
    // 캐릭터 무기 장착 토글 함수
    //void ToggleHandOnWeapon();
    

public:
    //작성 한승헌 일시 2025.11.13
    void OpenQuestUI(int32 QuestCode);
private:
    // 입력 매핑
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SuicideAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> AttackAction;



    //작성- 한승헌 일시 2025.11.12
    //NPC와 상호작용을 위한 키 추가.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> InteractionAction;

    // -작성: 노현기 -일시: 2025.11.14
    // 인벤토리 토글을 위한 키 추가
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> InventoryAction;

    // -작성: 노현기 -일시: 2025.11.14
    // 카타나 발도/납도 하기위한 키 추가
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> DrawKatanaAction;

    //작성 - 한승헌 2025-11-10
    //디버그용 데미지 주기.
    UFUNCTION(Exec)
    void DebugDamage(float Amount = 20.f);
private:
    // GAS 
    void HandleAbilityPressed(int32 InputID);
    void HandleAbilityReleased(int32 InputID);
    // 입력 처리 함수
    void HandleMove(const FInputActionValue& Value);
    void HandleLook(const FInputActionValue& Value);
    void HandleSuicide(const FInputActionValue& Value);

    // -작성: 노현기 -일시: 2025.11.17
    // 발도/납도 토글 처리
    void ToggleDrawSheath();

//HUD 생성 -작성: 한승헌 -일시: 2025.11.07
protected:
    //생성할 위젯의 클래스.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
    TSubclassOf<class UP4HUDWidget> P4HUDWidgetClass;

    //생성된 위젯의 객체.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
    TObjectPtr<class UP4HUDWidget> P4HUDWidget;

    // -작성: 노현기 -일시: 2025.11.10
    // 인벤토리 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
    TSubclassOf<class UP4InventoryWidget> InventoryWidgetClass;
    
//- 작성: 한승헌 일시: 2025.11.13
//퀘스트 UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    TSubclassOf<class UP4QuestWidget> QuestWidgetClass;

    UPROPERTY()
    TObjectPtr<UP4QuestWidget> QuestWidget;

// -작성: 노현기 -일시: 2025.11.10 
private:
    // 인벤토리 위젯 참조
    UPROPERTY()
    TObjectPtr<class UP4InventoryWidget> InventoryWidget;

    // 인벤토리 창 띄우기 = false
    bool bIsInventoryVisible = false;

    //// -작성: 노현기 -일시: 2025.11.14
    //// 캐릭터 무기 들고있는 상태 토글
    //bool bIsEquip = false;
};
