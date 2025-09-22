// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_PutDown.h"

UCAS_Task_PutDown* UCAS_Task_PutDown::Task_PutDown(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_PutDown* Task = NewAbilityTask<UCAS_Task_PutDown>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_PutDown::Activate()
{
	Super::Activate();

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	AbilityEndEvent.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}
