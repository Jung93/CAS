// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_PlayMontage.h"
#include "CAS_Ability_TakeDamage.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_TakeDamage : public UCAS_GameplayAbility
{
	GENERATED_BODY()
public:
	UCAS_Ability_TakeDamage();
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void MontageEndEvent(UAnimMontage* Montage, bool bInterrupted) override;
protected:
	UPROPERTY()
	UCAS_Task_PlayMontage* PlayMontageTask;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* TakeDamageMontage;
};
