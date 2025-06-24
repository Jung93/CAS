// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_Evade.generated.h"

UCLASS()
class CAS_API UCAS_Task_Evade : public UCAS_AbilityTask
{
	GENERATED_BODY()

public:
	static UCAS_Task_Evade* Task_Evade(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate = 1.0f);
protected:
	virtual void Activate() override;

};
