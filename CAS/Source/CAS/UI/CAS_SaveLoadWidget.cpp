// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Controller/CAS_PlayerController.h"
#include "Global/CAS_GameInstance.h"

void UCAS_SaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();	

	SelectionWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);
	if (SelectionWidget) {
		SelectionWidget->AddToViewport(10);
		CloseSelectionWidget();

		SelectionWidget->NO_OnClickedEvent(this, FName("CloseSelectionWidget"));
		SelectionWidget->YES_OnClickedEvent(this, FName("LoadFromSlot"));
	}
	
	CAS_ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseSaveLoadWidget);

	for (auto slot : SaveLoadSlots) {
		slot->SendSlotIndex.AddUObject(SelectionWidget, &UCAS_SelectWidget::SetSelectedIndex);
	}
	for (int32 i = 0; i < SlotCount; i++) {

		SaveLoadSlots[i]->BindOnClickedEvent(this, FName("DisplaySelectionWidget"));
		SaveLoadSlots[i]->SendSlotIndex.AddUObject(SelectionWidget, &UCAS_SelectWidget::SetSelectedIndex);
	}
}

void UCAS_SaveLoadWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
		
	SaveLoadSlots.SetNum(SlotCount);

	for (int32 i = 0; i < SlotCount; i++) {
		if (SlotWidgetClass) {
			UCAS_SaveLoadSlot* slot = CreateWidget<UCAS_SaveLoadSlot>(GetWorld(), SlotWidgetClass);
			slot->SetSlotIndex(i);
			CAS_VerticalBox->AddChild(slot);
			SaveLoadSlots[i] = slot;
		}
	}

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
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->EnterUIMode();
}

void UCAS_SaveLoadWidget::CloseSaveLoadWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->ExitUIMode();
}

void UCAS_SaveLoadWidget::LoadFromSlot()
{
	int32 index = SelectionWidget->GetSelectedIndex();

}
