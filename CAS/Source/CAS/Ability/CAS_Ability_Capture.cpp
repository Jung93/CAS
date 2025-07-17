// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Capture.h"
#include "Ability_Task/CAS_Task_Capture.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"

UCAS_Ability_Capture::UCAS_Ability_Capture()
{
}

bool UCAS_Ability_Capture::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_Capture::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_Capture::Task_Capture(this, "Capture");
	if (Task->IsValidLowLevel()) {
		Task->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();
	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", CaptureMontage, 1.0f, true);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}


}

void UCAS_Ability_Capture::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UCAS_Ability_Capture::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

	auto player = Cast<ACAS_Player>(GetActorInfo().AvatarActor);

	if (!player->IsValidLowLevel())
		return;

	ACAS_Hat* Hat = player->GetHat();


	if (NotifyName == "Capture1")
	{
		Hat->Ready();


	}
	else if (NotifyName == "Capture2")
	{
		auto dir = player->GetActorForwardVector();

		Hat->Throw(dir);
		Hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	else if (NotifyName == "Capture3")
	{
	}



	PlayMontageTask->ReadyForActivation();
}

void UCAS_Ability_Capture::MontageEndEvent(UAnimMontage* Montage, bool bInterrupted)
{
	CAS_EndAbility();
}
