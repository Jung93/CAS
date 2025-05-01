// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_Attack : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_Attack* CAS_Task_Attack(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate = 1.0f);

	virtual void Activate() override;
protected:

};
