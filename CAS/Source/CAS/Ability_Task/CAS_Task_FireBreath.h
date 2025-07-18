// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_FireBreath.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FAttackHitEvent, ACAS_Character*, int32);

UCLASS()
class CAS_API UCAS_Task_FireBreath : public UCAS_AbilityTask
{
	GENERATED_BODY()
	
public:
	static UCAS_Task_FireBreath* Task_FireBreath(UGameplayAbility* OwningAbility, FName TaskName, float Rate = 1.0f);

	FAttackHitEvent OnAttackHit;

protected:
	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

private:
	float TotalTime = 0.0f;
	float MaxDuration = 2.0f;

	UPROPERTY()
	TArray<ACAS_Character*> AlreadyHitActors;
};
