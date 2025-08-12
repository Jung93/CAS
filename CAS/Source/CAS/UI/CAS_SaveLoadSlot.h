// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CAS_SaveLoadSlot.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SaveLoadSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSlotIndex(int32 index) { SlotIndex = index; }
	int32 GetSlotIndex() { return SlotIndex; }

	UButton* GetSlotButton() { return CAS_SaveLoadButton; }
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* CAS_SaveLoadButton;

	int32 SlotIndex;
};
