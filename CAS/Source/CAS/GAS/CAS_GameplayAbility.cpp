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
	auto DefaultObj = GetClass()->GetDefaultObject<UCAS_GameplayAbility>();

	UTexture2D* TextureData = DefaultObj->AbilityIcon;
	EAbilityInputID InputID_Data = DefaultObj->InputID;
	FName TagData = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();

	FCAS_SkillData Data;

	Data.AbilityIconTexture = TextureData;
	Data.AbilityTag = TagData;
	Data.InputID = InputID_Data;

	SkillData = Data;
}

void UCAS_GameplayAbility::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}

void UCAS_GameplayAbility::MontageEndEvent(UAnimMontage* Montage, bool bInterrupted)
{
}

void UCAS_GameplayAbility::CAS_EndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
