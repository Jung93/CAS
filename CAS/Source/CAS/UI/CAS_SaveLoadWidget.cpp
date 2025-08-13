// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadWidget.h"
#include "Components/VerticalBox.h"
#include "Controller/CAS_PlayerController.h"

void UCAS_SaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();
		
	SelectionWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);
	if (SelectionWidget) {
		CloseSelectionWidget();
	}
	
	CAS_ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseSaveLoadWidget);
	
}

void UCAS_SaveLoadWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SaveLoadSlots.SetNum(SlotCount);

	for (int32 i = 0; i < SlotCount; i++) {
		if (SlotWidgetClass) {
			UCAS_SaveLoadSlot* slot = CreateWidget<UCAS_SaveLoadSlot>(GetWorld(), SlotWidgetClass);
			slot->SetSlotIndex(i);
			slot->BindOnClickedEvent(this, FName("DisplaySelectionWidget"));
			slot->SendSlotIndex.AddUObject(SelectionWidget, &UCAS_SelectWidget::SetSellectedIndex);
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
