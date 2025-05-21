// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GAS/CAS_GameplayAbility.h"
#include "CAS_AbilityTask.generated.h"

DECLARE_MULTICAST_DELEGATE_FiveParams(FAbilityEndEvent, FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, FGameplayAbilityActivationInfo, bool, bool);

UCLASS()
class CAS_API UCAS_AbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	FAbilityEndEvent OnAbilityEnd;
protected:
	UFUNCTION()
	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;
	UPROPERTY()
	float PlayRate = 1.0f;
};

/*
체력이 0되면 델리게이트로 플레이어의 데드어빌리티 실행
데드어빌리티 -> 즉시 데드 태그를 달고 
->태스크실행 ->애님노티파이로 사라지게처리->어빌리티 종료

데드태그 -> 행동트리에서 막아야함 , 데드태그를 달면 하던공격취소 및 데미지 x


*/
