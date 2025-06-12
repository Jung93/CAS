// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_StealAbility.generated.h"

UCLASS()
class CAS_API UCAS_Task_StealAbility : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_StealAbility* Task_StealAbility(UGameplayAbility* OwningAbility, FName TaskName);

protected:
	virtual void Activate() override;

};
