// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_GameplayAbility.h"
#include "Ability_Task/CAS_Task_PlayMontage.h"
#include "CAS_Ability_Push.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Ability_Push : public UCAS_GameplayAbility
{
	GENERATED_BODY()
public:
	UCAS_Ability_Push();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;

	FActiveGameplayEffectHandle  ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent);
	void ReceiveTarget(ACAS_Character* Target, int32 TaskLevel);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* PushMontage;

	UPROPERTY()
	UCAS_Task_PlayMontage* PlayMontageTask;

	FActiveGameplayEffectHandle GetActiveEffectHandle() { return ActiveEffectHandle; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> TagEffectClassJump;

private:
	UPROPERTY()
	FActiveGameplayEffectHandle ActiveEffectHandle;


};
