// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_StealAbility.h"

UCAS_Task_StealAbility* UCAS_Task_StealAbility::Task_StealAbility(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_StealAbility* Task = NewAbilityTask<UCAS_Task_StealAbility>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_StealAbility::Activate()
{


	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	AbilityEndEvent.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);

}
