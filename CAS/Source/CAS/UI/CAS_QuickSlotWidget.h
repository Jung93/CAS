// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/CAS_SkillSlot.h"
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
	void SwapSlots(UCAS_SkillSlot* DragSlot, UCAS_SkillSlot* DropSlot);
	const FCAS_SlotData& GetSlotData(int32 SlotIndex) const;
protected:
	virtual void NativeConstruct() override;

	int32 SlotCount;
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UHorizontalBox* SlotBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UCAS_SkillSlot> SlotWidgetClass;
	UPROPERTY()
	TArray<class UCAS_SkillSlot*> SlotWidgets;
	UPROPERTY()
	TArray<FCAS_SlotData> SlotData;
};
