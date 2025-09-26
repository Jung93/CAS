// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_Push.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_Push : public UCAS_AbilityTask
{
	GENERATED_BODY()

public:
	static UCAS_Task_Push* Task_Push(UGameplayAbility* OwningAbility, FName TaskName);

protected:
	virtual void Activate() override;

};
