// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_ChargeInput.h"

UCAS_Task_ChargeInput* UCAS_Task_ChargeInput::CAS_Task_ChargeInput(UGameplayAbility* OwningAbility, FName TaskName, float MaxChargeTime)
{
	UCAS_Task_ChargeInput* Task = NewAbilityTask<UCAS_Task_ChargeInput>(OwningAbility, TaskName);

	Task->MaxChargeTime = MaxChargeTime;
	Task->StartTime = 0.0f;
	return Task;
}

void UCAS_Task_ChargeInput::Activate()
{
	Super::Activate();

	if (!Ability || !Ability->GetCurrentActorInfo())
	{
		EndTask();
		return;
	}
	StartTime = GetWorld()->GetTimeSeconds();

	if (MaxChargeTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UCAS_Task_ChargeInput::ChargeReleased,
			MaxChargeTime,
			false
		);
	}
	
}

void UCAS_Task_ChargeInput::ChargeReleased()
{
	//auto ChargeTime = 
	//ChargeReleaseEvent.Broadcast(ChargeTime);
}
