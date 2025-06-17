// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_GameplayAbility.h"

UCAS_GameplayAbility::UCAS_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCAS_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
}

void UCAS_GameplayAbility::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}

void UCAS_GameplayAbility::CAS_EndAbility()
{
	auto Handle = this->GetCurrentAbilitySpecHandle();
	auto ActorInfo = this->GetCurrentActorInfo();
	auto ActivationInfo = this->GetCurrentActivationInfo();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

}
