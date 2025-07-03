// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_LoopMontage.generated.h"


UCLASS()
class CAS_API UCAS_Task_LoopMontage : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_LoopMontage* Task_LoopMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate = 1.0f);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished)override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;
	UPROPERTY()
	float PlayRate = 1.0f;

protected:
	void CheckMontageLoop();

	FTimerHandle MontageLoopTimerHandle;

	bool bStopRequested = false;
};
