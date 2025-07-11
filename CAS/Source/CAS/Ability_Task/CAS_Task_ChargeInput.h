// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_ChargeInput.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChargeReleaseEvent, float);

UCLASS()
class CAS_API UCAS_Task_ChargeInput : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_ChargeInput* CAS_Task_ChargeInput(UGameplayAbility* OwningAbility, FName TaskName,float MaxChargeTime);
	FChargeReleaseEvent ChargeReleaseEvent;

	void ChargeReleased(int32 ReleasedTime);
	void ChargeReleased();
protected:
	virtual void Activate() override;
	float MaxChargeTime;
	float StartTime;
	FTimerHandle TimerHandle;

};
