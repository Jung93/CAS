// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_Attack.h"
#include "Ability_Task/CAS_Task_PlayMontage.h"
#include "CAS_Ability_TESTFUNC.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_TESTFUNC : public UCAS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UCAS_Ability_TESTFUNC();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent);
	void ReceiveTarget(ACAS_Character* Target, int32 TaskLevel);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> TagEffectClass;
	UPROPERTY()
	UCAS_Task_Attack* AttackTask;
	UPROPERTY()
	UCAS_Task_PlayMontage* PlayMontageTask;

public:	
	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
