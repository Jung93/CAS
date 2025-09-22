// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_PickUp.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_PickUp : public UCAS_AbilityTask
{
	GENERATED_BODY()
	
public:
	static UCAS_Task_PickUp* Task_PickUp(UGameplayAbility* OwningAbility, FName TaskName);

protected:
	virtual void Activate() override;

};
