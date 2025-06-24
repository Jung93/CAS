// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_PlayMontage : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_PlayMontage* Task_PlayMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate = 1.0f , bool bNotifyReady = false);
protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished)override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;
	UPROPERTY()
	float PlayRate = 1.0f;
	bool bNotifyReady = false;
};
