// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_GameplayAbility.h"

UCAS_GameplayAbility::UCAS_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/GamePlayEffect/GE_Detectable_Tag.GE_Detectable_Tag_C'"));

	if (EffectClass.Succeeded())
	{
		DetectableTagEffectClass = EffectClass.Class;
	}
}

const FCAS_SkillData& UCAS_GameplayAbility::GetSkillData()
{
	if (SkillData.AbilityIconTexture == nullptr) {
		FCAS_SkillData Data;

		UTexture2D* TextureData = AbilityIcon;
		EAbilityInputID InputID_Data = InputID;
		FName TagData = AbilityTags.GetByIndex(0).GetTagName();


		Data.AbilityIconTexture = TextureData;
		Data.AbilityTag = TagData;
		Data.InputID = InputID_Data;

		SkillData = Data;
	}
	return SkillData;
}

void UCAS_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DetectableTagEffectClass, 1);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}
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
