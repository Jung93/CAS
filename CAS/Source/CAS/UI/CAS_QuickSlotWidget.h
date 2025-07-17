// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/CAS_SkillSlot.h"
#include "GAS/CAS_GameplayAbility.h"
#include "CAS_QuickSlotWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FQuickSlotSwapEvent,int32,FCAS_SlotData );
DECLARE_MULTICAST_DELEGATE_OneParam(FRemoveAbilityEvent, int32);


UCLASS()
class CAS_API UCAS_QuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitSetting(int32 count);
	void SwapSlots(UCAS_SkillSlot* DragSlot, UCAS_SkillSlot* DropSlot);
	const FCAS_SlotData& GetSlotData(int32 SlotIndex) const { return SkillSlots[SlotIndex]->GetSlotData(); }
	void SetSlotData(int32 index, const FCAS_SlotData& AbilityData);
	void RemoveSlotData(int32 index);

	const TArray<UCAS_SkillSlot*> GetSkillSlots() { return SkillSlots; }

	void OpenSlot();
	void CloseSlot();

	void ChangeSlotToLeft();
	void ChangeSlotToRight();

	void BlockSlotSwap(TArray<UCAS_SkillSlot*> SelectSkillSlots);


	FQuickSlotSwapEvent QuickSlotSwapEvent;
	FRemoveAbilityEvent RemoveAbilityEvent;


protected:
	int32 SlotCount;
protected:
	//UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	//class UHorizontalBox* SlotBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UCAS_SkillSlot> SlotWidgetClass;
	UPROPERTY()
	TArray<UCAS_SkillSlot*> SkillSlots;
	
	float BorderPosX = 0.0f;
	float BorderSizeX = 350.0f;
};
