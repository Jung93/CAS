// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "CAS_Ability_FireBreath.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_FireBreath : public UCAS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UCAS_Ability_FireBreath();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	FActiveGameplayEffectHandle  ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent);
	FActiveGameplayEffectHandle  ApplyGamePlayEffectToTarget(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent);
	void ReceiveTarget(ACAS_Character* Target, int32 TaskLevel);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> TagEffectClass;
};
