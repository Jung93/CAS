// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"	
#include "Components/TextBlock.h"
#include "CAS_SaveLoadSlot.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSendSlotIndex, int32);

UCLASS()
class CAS_API UCAS_SaveLoadSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetSlotIndex(int32 index) { SlotIndex = index; }
	int32 GetSlotIndex() { return SlotIndex; }

	void SetSlotText(int32 Slotindex);

	void SetButtonText(FText text);
	void SetButtonText(FString string);

	void BindOnClickedEvent(UObject* Object, FName name);
	FSendSlotIndex SendSlotIndex;

protected:
	UFUNCTION()
	void OnClickedEvent();
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* CAS_SaveLoadButton;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CAS_TextBlock;

	int32 SlotIndex;

};
