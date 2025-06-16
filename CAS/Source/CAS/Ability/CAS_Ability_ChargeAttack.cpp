// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_ChargeAttack.h"
#include "Ability_Task/CAS_Task_ChargeInput.h"

UCAS_Ability_ChargeAttack::UCAS_Ability_ChargeAttack()
{
}

void UCAS_Ability_ChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//auto WaitRelease = UCAS_Task_ChargeInput::CAS_Task_ChargeInput(this,"ChargeInput",);
	//WaitRelease->ChargeReleaseEvent.AddDynamic(this, &UGA_VaultBreaker::OnChargeReleased);
	//WaitRelease->ReadyForActivation();
}

void UCAS_Ability_ChargeAttack::OnChargeReleased(float ChargeTime)
{
}
