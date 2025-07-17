// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Capture.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"
UCAS_Task_Capture* UCAS_Task_Capture::Task_Capture(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_Capture* Task = NewAbilityTask<UCAS_Task_Capture>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_Capture::Activate()
{
	Super::Activate();

}


