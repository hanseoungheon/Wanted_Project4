// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/P4NPCBase.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

#include "Character/P4CharacterPlayer.h"

#include "Quest/P4QuestInfo.h"
#include "Quest/P4StageDetails.h"
#include "Quest/P4ObjectiveDetails.h"

#include "Animation/AnimMontage.h"

#include "UI/P4QuestWidget.h"
#include "UI/P4EnchantWidget.h"

// Sets default values
AP4NPCBase::AP4NPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(350.0f);

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionSphere->SetGenerateOverlapEvents(true);

	InteractionSphere->bHiddenInGame = false;

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AP4NPCBase::OnOverlapBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AP4NPCBase::OnOverlapEnd);

	//Tag_InRange = FGameplayTag::RequestGameplayTag(FName("Character.Interaction"));

	static ConstructorHelpers::FClassFinder<UP4QuestWidget> QuestWidgetClassRef(TEXT("/Game/UI/WBP_QuestWidget.WBP_QuestWidget_C"));

	if (QuestWidgetClassRef.Succeeded() == true)
	{
		QuestWidgetClass = QuestWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UP4EnchantWidget> EnchantWidgetClassRef(TEXT("/Game/UI/WBP_Enchant.WBP_Enchant_C"));

	if (EnchantWidgetClassRef.Succeeded() == true)
	{
		EnchantWidgetClass = EnchantWidgetClassRef.Class;
	}
}

// Called when the game starts or when spawned
void AP4NPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP4NPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP4NPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP4NPCBase::Interaction(ACharacter* Character)
{
	AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(Character);

	if (PlayerCharacter == nullptr)
	{
		return;
	}
}

void AP4NPCBase::ShowQuestUI(int32 QuestCode)
{
	APlayerController* PlayerController =
		Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (QuestWidgetClass == nullptr || PlayerController == nullptr)
	{
		return;
	}

	if (QuestWidgetInstance == nullptr)
	{
		QuestWidgetInstance = CreateWidget<UP4QuestWidget>(PlayerController, QuestWidgetClass);
	}

	if (QuestWidgetInstance != nullptr)
	{
		QuestWidgetInstance->AddToViewport(10);
		QuestWidgetInstance->InitWithQuestCode(QuestCode);

		PlayerController->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(QuestWidgetInstance->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
}

void AP4NPCBase::ShowEnchantUI()
{
	APlayerController* PlayerController
		= Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (EnchantWidgetClass == nullptr ||PlayerController == nullptr)
	{
		return;
	}

	//EnchantWidgetInstance가 nullptr일경우 즉각할당.
	if (EnchantWidgetInstance == nullptr)
	{
		EnchantWidgetInstance 
			= CreateWidget<UP4EnchantWidget>(PlayerController, EnchantWidgetClass);

		if (EnchantWidgetInstance != nullptr)
		{
			TScriptInterface<IP4NPCEnchantWeaponInterface> SelfInterface;
			SelfInterface.SetObject(this);
			SelfInterface.SetInterface(Cast<IP4NPCEnchantWeaponInterface>(this));

			EnchantWidgetInstance->SetEnchantNPC(SelfInterface);
		}
	}

	if (EnchantWidgetInstance != nullptr)
	{
		EnchantWidgetInstance->AddToViewport(9);

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(EnchantWidgetInstance->TakeWidget());
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}


void AP4NPCBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false)
	{
		return;
	}


	AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(OtherActor);
	OverlapedActor = PlayerCharacter;
	//UE_LOG(LogTemp, Display, TEXT("IsOverlapped"));

	if (PlayerCharacter == nullptr)
	{
		OverlapedActor = nullptr;
		return; //플레이어가 아니면 바로 리턴시키기.
	}


	//UE_LOG(LogTemp, Display, TEXT("IsOverlapped"));
	IAbilitySystemInterface* GASInterface = Cast<IAbilitySystemInterface>(PlayerCharacter);

	if (GASInterface != nullptr)
	{
		UAbilitySystemComponent* PlayerASC = GASInterface->GetAbilitySystemComponent();

		PlayerASC->AddLooseGameplayTag(Tag_InRange); //소통범위 안에있으면 NPC와 소통가능하게 태그 부착.
	}

	PlayerCharacter->CurrentInteractActor = this;
}

void AP4NPCBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() == false)
	{
		return;
	}

	AP4CharacterPlayer* PlayerCharacter = Cast<AP4CharacterPlayer>(OtherActor);

	//UE_LOG(LogTemp, Display, TEXT("IsEndOverlapped"));

	if (PlayerCharacter == nullptr)
	{
		OverlapedActor = nullptr;
		return;
	}

	OverlapedActor = nullptr;

	//UE_LOG(LogTemp, Display, TEXT("IsEndOverlapped"));
	IAbilitySystemInterface* GASInterface = Cast<IAbilitySystemInterface>(PlayerCharacter);

	if (GASInterface != nullptr)
	{
		UAbilitySystemComponent* PlayerASC = GASInterface->GetAbilitySystemComponent();

		PlayerASC->RemoveLooseGameplayTag(Tag_InRange); //소통범위 밖에 있으면 NPC와 소통 불가능하게 태그 탈착.
	}

	PlayerCharacter->CurrentInteractActor = nullptr;
}


UAbilitySystemComponent* AP4NPCBase::GetAbilitySystemComponent() const
{
	return nullptr;
}

