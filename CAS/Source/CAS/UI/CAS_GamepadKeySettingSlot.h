// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CAS_KeySettingSlot.h"
#include "CAS_GamepadKeySettingSlot.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_GamepadKeySettingSlot : public UCAS_KeySettingSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSelect(FInputChord NewKey);

};
