// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "CAS_InteractionSwitch.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwitchClicked, bool);

UCLASS()
class CAS_API ACAS_InteractionSwitch : public ACAS_InteractionActor
{
	GENERATED_BODY()
public:
	virtual void InteractionWithPlayer() override;

	bool IsSwitchOn() { return bSwitchOn; }

	FOnSwitchClicked OnSwitchClicked;
protected:
	virtual void BeginPlay() override;

private:

	bool bSwitchOn = false;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};






