// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_ChargeInput.h"

UCAS_Task_ChargeInput* UCAS_Task_ChargeInput::CAS_Task_ChargeInput(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float MaxChargeTime, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_ChargeInput* Task = NewAbilityTask<UCAS_Task_ChargeInput>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;
	Task->MaxCharageTime = MaxChargeTime;
	Task->StartTime = OwningAbility->GetWorld()->GetTimeSeconds();
	return Task;
}

void UCAS_Task_ChargeInput::Activate()
{
	Super::Activate();

	//auto Character = Cast<ACAS_Character>(Ability->GetOwningActorFromActorInfo());
	//auto ASC = Character->GetAbilitySystemComponent();
	//
	//ASC->IsInputPressed(Ability->GetCurrentAbilitySpecHandle()
	//ASC->AbilitySpecInputPressed
	//
	//ChargeReleaseEvent.Broadcast(ChargeTime);
}
