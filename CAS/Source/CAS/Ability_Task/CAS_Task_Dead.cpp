// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Dead.h"

UCAS_Task_Dead* UCAS_Task_Dead::Task_Dead(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Dead* Task = NewAbilityTask<UCAS_Task_Dead>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Dead::Activate()
{
	Super::Activate();
	if (!Montage) {
		return;
	}
}

void UCAS_Task_Dead::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}
