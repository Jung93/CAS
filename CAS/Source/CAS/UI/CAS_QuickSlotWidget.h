// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAS_QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_QuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitSetting(int32 count);
protected:
	virtual void NativeConstruct() override;
	int32 SlotCount;
};
