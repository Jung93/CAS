// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_ChargeAttack.h"


UCAS_Ability_ChargeAttack::UCAS_Ability_ChargeAttack()
{
}

void UCAS_Ability_ChargeAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	StartTime = GetWorld()->GetTimeSeconds();

}

void UCAS_Ability_ChargeAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndTime = GetWorld()->GetTimeSeconds();
	ChargedTime = EndTime - StartTime;

	FString DebugMessage = FString::Printf(TEXT("ChargeTime : %d"), ChargedTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, DebugMessage);
	//ChargeInputTask->ChargeReleased(chargeTime);
}

void UCAS_Ability_ChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//ChargeInputTask = UCAS_Task_ChargeInput::CAS_Task_ChargeInput(this, "ChargeInput",4.0f );
	//
	//if (ChargeInputTask) {
	//	ChargeInputTask->ChargeReleaseEvent.AddUObject(this, &ThisClass::OnChargeReleased);
	//	ChargeInputTask->ReadyForActivation();
	//}
}

void UCAS_Ability_ChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//ChargeInputTask->EndTask();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_ChargeAttack::OnChargeReleased(float ChargeTime)
{
	FString DebugMessage = FString::Printf(TEXT("ChargeTime : %d"),ChargeTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, DebugMessage);

	CAS_EndAbility();
}
