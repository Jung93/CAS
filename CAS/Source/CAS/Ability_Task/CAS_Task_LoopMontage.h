// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_LoopMontage.generated.h"

DECLARE_MULTICAST_DELEGATE(FMontageEndEvent);

UCLASS()
class CAS_API UCAS_Task_LoopMontage : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_LoopMontage* Task_LoopMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, FName StartSection = "NAME_None", float Rate = 1.0f);
	FMontageEndEvent MontageEndEvent;
protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished)override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;
	UPROPERTY()
	float PlayRate = 1.0f;

protected:
	void PlayMontage();
	void CheckMontageLoop();
	void StopMontage();

	FTimerHandle MontageLoopTimerHandle;

	FName SectionName;

	bool bStopRequested = false;
};
