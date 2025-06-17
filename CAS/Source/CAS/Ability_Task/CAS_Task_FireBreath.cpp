// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_FireBreath.h"

UCAS_Task_FireBreath* UCAS_Task_FireBreath::Task_FireBreath(UGameplayAbility* OwningAbility, FName TaskName, float Rate)
{
	UCAS_Task_FireBreath* Task = NewAbilityTask<UCAS_Task_FireBreath>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_FireBreath::Activate()
{
	Super::Activate();


	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCAS_Task_FireBreath::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}
