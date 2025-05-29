// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SkillSlot.h"
#include "UI/CAS_QuickSlotWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"

void UCAS_SkillSlot::SetSlotData(const FCAS_SlotData& InitData)
{
	SlotData = InitData;
	UpdateIcon();
}

void UCAS_SkillSlot::UpdateIcon()
{
	UTexture2D* Texture;
	if (SlotData.SlotTexture)
	{
		Texture = SlotData.SlotTexture;
	}
	else {
		Texture = DefaultTexture;
	}
	CAS_Image->SetBrushFromTexture(Texture);
}

FReply UCAS_SkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCAS_SkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->Payload = this;
	DragOp->DefaultDragVisual = this;
	OutOperation = DragOp;
}

bool UCAS_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (auto otherSlot = Cast<UCAS_SkillSlot>(InOperation->Payload))
	{
		if (UCAS_QuickSlotWidget* quickSlotWidget = Cast<UCAS_QuickSlotWidget>(GetParent()))
		{
			quickSlotWidget->SwapSlots(this, otherSlot);
			return true;
		}
	}
	return false;
}
