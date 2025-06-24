// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_ChargeInput.h"
#include "CAS_Ability_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_ChargeAttack : public UCAS_GameplayAbility
{
	GENERATED_BODY()
public:
	UCAS_Ability_ChargeAttack();

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnChargeReleased(float HeldTime);

	UPROPERTY(EditDefaultsOnly, Category = "ChargeAttack")
	float MaxChargeTime = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ChargeAttack")
	UAnimMontage* ChargeAttackMontage;

protected:
	//UCAS_Task_ChargeInput* ChargeInputTask;
	
	float StartTime = 0.0f; 
	float EndTime = 0.0f;
	float ChargedTime = 0.0f;
};
