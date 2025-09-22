// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_PutDown.h"
#include "Ability_Task/CAS_Task_PutDown.h"
#include "Controller/CAS_PlayerController.h"
#include "Character/CAS_Player.h"


UCAS_Ability_PutDown::UCAS_Ability_PutDown()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_PutDown::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

}

void UCAS_Ability_PutDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_PutDown::Task_PutDown(this, "PutDown");

	if (Task->IsValidLowLevel())
	{
		Task->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		ReceiveTarget(owner, 1);

	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", CaptureMontage, 1.0f, true);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_PutDown::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_PutDown::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	PlayMontageTask->ReadyForActivation();
}

FActiveGameplayEffectHandle UCAS_Ability_PutDown::ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
		return Handle;
	}

	return FActiveGameplayEffectHandle();
}

void UCAS_Ability_PutDown::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
}
