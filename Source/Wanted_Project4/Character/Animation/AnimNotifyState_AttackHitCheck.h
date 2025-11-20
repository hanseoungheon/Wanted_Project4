// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class WANTED_PROJECT4_API UAnimNotifyState_AttackHitCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AttackHitCheck();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	/** 데미지 배율(기존 AnimNotify DamageMultiplier와 동일 역할) */
	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageMultiplier = 1.0f;

	/** 이번 공격 동안 이미 맞은 Actor 저장 */
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActors;

	/** Trace Debug 표시 여부 */
	UPROPERTY()
	bool bShowDebug = true;

	/** (옵션) AT_Trace 방식 대신 직접 Trace 구현 */
	void PerformTrace(class USkeletalMeshComponent* MeshComp);
};
