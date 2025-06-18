// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_PlayMontage.h"
#include "CAS_Ability_Evade.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_Evade : public UCAS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UCAS_Ability_Evade();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;

protected:
	UPROPERTY()
	UCAS_Task_PlayMontage* PlayMontageTask;

public:

};
