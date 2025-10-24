// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Evade.h"
#include "Ability_Task/CAS_Task_Evade.h"
#include "Character/CAS_Player.h"

UCAS_Ability_Evade::UCAS_Ability_Evade()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_Evade::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	if (isPlaying) {
		return false;
	}
	return true;
}

void UCAS_Ability_Evade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AttackMontage) {
		return;
	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "Evade", AttackMontage, 1.5f,true);
	if (PlayMontageTask->IsValidLowLevel()) {
		PlayMontageTask->TaskEndEvent.AddUObject(this, &ThisClass::CAS_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}


	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = nullptr;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();
		AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Player"));
	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
			AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));

		}
	}





}

void UCAS_Ability_Evade::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_Evade::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	PlayMontageTask->TaskEndEvent.Broadcast();
}
