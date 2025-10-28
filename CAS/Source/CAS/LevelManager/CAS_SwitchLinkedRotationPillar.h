// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_SwitchLinkedPillar.h"
#include "CAS_SwitchLinkedRotationPillar.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_SwitchLinkedRotationPillar : public ACAS_SwitchLinkedPillar
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
	virtual void SwitchClicked(bool SwitchOn) override;

	void SetTargetRotation();

private:
	FRotator StartRotation;
	FRotator EndRotation;
};
