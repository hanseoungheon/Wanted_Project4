#include "Character/Animation/AnimNotifyState_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/P4CharacterPlayer.h"
//#include "Character/GA/TA/P4TA_Trace.h"
#include "Attribute/P4PlayerAttributeSet.h"
#include "Interface/P4DamageableInterface.h"
#include "DrawDebugHelpers.h"
#include "Item/Equipment/P4WeaponComponent.h"
#include "Physics/P4Collision.h"

UAnimNotifyState_AttackHitCheck::UAnimNotifyState_AttackHitCheck()
{
}

void UAnimNotifyState_AttackHitCheck::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	HitActors.Empty();
}

void UAnimNotifyState_AttackHitCheck::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	PerformTrace(MeshComp);
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	HitActors.Empty();
}

/** Trace 로직 */
void UAnimNotifyState_AttackHitCheck::PerformTrace(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp) return;

	AP4CharacterPlayer* P4Player = Cast<AP4CharacterPlayer>(MeshComp->GetOwner());
	if (!P4Player) return;

	//UAbilitySystemComponent* ASC = P4Player->GetAbilitySystemComponent();
	//if (!ASC) return;

	USkeletalMeshComponent* Weapon = P4Player->GetWeaponComponent()->WeaponMesh;

	// todo: 칼의 판정 반지름을 받아오도록??
	float Radius = 30.f;

	FVector Start = Weapon->GetSocketLocation("StartSocket");
	FVector End = Weapon->GetSocketLocation("EndSocket");

	// 플레이어 판정 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AttackTrace), false, P4Player);
	TArray<FHitResult> Hits;

	bool HitDetected = P4Player->GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, CCHANNEL_P4ACTION, FCollisionShape::MakeSphere(Radius), Params);

	if (bShowDebug)
	{
		FVector SweepVector = End - Start;
		float SweepLength = SweepVector.Size();
		FVector Center = Start + (SweepVector * 0.5f);
		float HalfHeight = (SweepLength * 0.5f) + Radius;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugCapsule(P4Player->GetWorld(), Center, HalfHeight, Radius, FRotationMatrix::MakeFromZ(SweepVector).ToQuat(), DrawColor, false, 1.f);
	}

	if (!HitDetected) return;

	UAbilitySystemComponent* ASC = P4Player->GetAbilitySystemComponent();
	if (!ASC) return;

	const UP4PlayerAttributeSet* PlayerAttribute = ASC->GetSet<UP4PlayerAttributeSet>();
	if (!PlayerAttribute) return;

	// todo: 플레이어 공격력? 카타나 공격력?
	const float Damage = PlayerAttribute->GetAttackRate();

	for (auto& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		// 이미 맞은 Actor는 무시
		if (!HitActor || HitActors.Contains(HitActor))
			continue;

		HitActors.Add(HitActor);

		if (IP4DamageableInterface* Damageable = Cast<IP4DamageableInterface>(HitActor))
		{
			Damageable->ApplyDamage(Damage);
		}
	}
}