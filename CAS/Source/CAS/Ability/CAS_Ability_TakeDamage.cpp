// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TakeDamage.h"

UCAS_Ability_TakeDamage::UCAS_Ability_TakeDamage()
{
}

bool UCAS_Ability_TakeDamage::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_TakeDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", TakeDamageMontage, 0.5f, false);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_TakeDamage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UCAS_Ability_TakeDamage::MontageEndEvent(UAnimMontage* Montage, bool bInterrupted)
{
	CAS_EndAbility();
}
