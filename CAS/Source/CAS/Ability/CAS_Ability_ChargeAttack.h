// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_Attack.h"
#include "Ability_Task/CAS_Task_PlayMontage.h"
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

protected:
	UCAS_Task_ChargeInput* ChargeInputTask;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* ChargeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> TagEffectClass;
	UPROPERTY()
	UCAS_Task_Attack* AttackTask;
	UPROPERTY()
	UCAS_Task_PlayMontage* ChargeMontageTask;
	UPROPERTY()
	UCAS_Task_PlayMontage* AttackMontageTask;
protected:

	float StartTime = 0.0f; 
	float EndTime = 0.0f;
	float ChargedTime = 0.0f;
};
