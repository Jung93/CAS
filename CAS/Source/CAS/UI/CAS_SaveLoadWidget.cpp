// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadWidget.h"
#include "Components/VerticalBox.h"

void UCAS_SaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SaveLoadSlots.SetNum(SlotCount);

	SelectionWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);
	if (SelectionWidget) {
		CloseSelectionWidget();
	}
	for (int32 i = 0; i < SlotCount; i++) {
		if (SlotWidgetClass) {
			UCAS_SaveLoadSlot* slot = CreateWidget<UCAS_SaveLoadSlot>(GetWorld(), SlotWidgetClass);
			slot->SetSlotIndex(i);
			auto Button = slot->GetSlotButton();
			Button->OnClicked.AddDynamic(this,&ThisClass::DisplaySelectionWidget);

			CAS_VerticalBox->AddChild(slot);
			SaveLoadSlots[i] = slot;
		}
	}
	
	CAS_ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseDisplaySaveLoadWidget);
	
}

void UCAS_SaveLoadWidget::InitSetting()
{
}

void UCAS_SaveLoadWidget::DisplaySelectionWidget()
{
	if (!SelectionWidget) {
		return;
	}
	SelectionWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCAS_SaveLoadWidget::CloseSelectionWidget()
{
	if (!SelectionWidget) {
		return;
	}
	SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UCAS_SaveLoadWidget::DisplaySaveLoadWidget()
{	
	SetVisibility(ESlateVisibility::Visible);
}

void UCAS_SaveLoadWidget::CloseDisplaySaveLoadWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
