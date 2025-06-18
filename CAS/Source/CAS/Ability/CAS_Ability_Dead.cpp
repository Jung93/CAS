// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Dead.h"
#include "Ability_Task/CAS_Task_Dead.h"

UCAS_Ability_Dead::UCAS_Ability_Dead()
{
}

bool UCAS_Ability_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeadMontage);
	if (isPlaying) {
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!DeadMontage) {
		return;
	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "Dead", DeadMontage, 1.0f,true);
	if (PlayMontageTask->IsValidLowLevel()) {
		PlayMontageTask->TaskEndEvent.AddUObject(this, &ThisClass::CAS_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
void UCAS_Ability_Dead::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto Character = Cast<ACAS_Character>(GetAvatarActorFromActorInfo());

	Character->SetActorHiddenInGame(true);
	Character->SetActorEnableCollision(false);

	auto ASC = Character->GetAbilitySystemComponent();
	ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));

	Character->Controller->UnPossess();

	PlayMontageTask->TaskEndEvent.Broadcast();
}
