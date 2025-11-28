// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyNoCapt.h"
#include "CAS_Dancer.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_Dancer : public ACAS_EnemyNoCapt
{
	GENERATED_BODY()
public:
	ACAS_Dancer();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MontageLoop(class UAnimMontage* MontageToLoop, bool IsInterupt);

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay;

};
