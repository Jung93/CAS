// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Controller/CAS_PlayerController.h"
#include "Global/CAS_GameInstance.h"
#include "Character/CAS_Player.h"
#include "UI/CAS_TitleWidget.h"
#include "Kismet/GameplayStatics.h"

void UCAS_SaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();	
					
	SelectionWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);
	OverwriteWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);

	if (SelectionWidget) {
		SelectionWidget->AddToViewport(8);

		SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);

		SelectionWidget->NO_OnClickedEvent(this, FName("CloseSelectionWidget"));
		SelectionWidget->YES_OnClickedEvent(this, FName("SaveLoadFromSlot"));

		if (bSaveMode) {
			SelectionWidget->SetWidgetText(TEXT(" Do you want to Save the Game ? "));
		}
		else {
			SelectionWidget->SetWidgetText(TEXT(" Do you want to Load the Game ? "));
		}
	}

	if (OverwriteWidget) {
		OverwriteWidget->AddToViewport(9);

		OverwriteWidget->SetVisibility(ESlateVisibility::Collapsed);

		OverwriteWidget->NO_OnClickedEvent(this, FName("CloseOverwriteWidget"));
		OverwriteWidget->YES_OnClickedEvent(this, FName("OverwriteSlot"));

		OverwriteWidget->SetWidgetText(TEXT("Do you want to Overwrite the Save ?"));

	}
	CAS_ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseSaveLoadWidget);
	
	for (int32 i = 0; i < SlotCount; i++) {

		SaveLoadSlots[i]->BindOnClickedEvent(this, FName("DisplaySelectionWidget"));
		SaveLoadSlots[i]->UpdateSlotInfo(i);
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

void UCAS_SaveLoadWidget::InitialSetting(bool IsSaveMode)
{
	bSaveMode = IsSaveMode;
	AddToViewport(5);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCAS_SaveLoadWidget::DisplaySelectionWidget()
{
	if (!SelectionWidget) {
		return;
	}
	SelectionWidget->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->SetKeyboardFocus();
}

void UCAS_SaveLoadWidget::CloseSelectionWidget()
{
	if (!SelectionWidget) {
		return;
	}
	SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);

	SetKeyboardFocus();
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

	if (PrevOpenedWidget)
	{
		PrevOpenedWidget->SetKeyboardFocus();
	}


	auto titleWidget = playerController->GetTitleWidget();
	if(titleWidget && titleWidget->IsVisible())
		return;

	playerController->ExitUIMode();


}

void UCAS_SaveLoadWidget::OverwriteSlot()
{
	auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());

	auto pawn = GetOwningPlayer()->GetPawn();

	if (!pawn || !GameInstance) {
		return;
	}

	auto player = Cast<ACAS_Player>(pawn);

	if (!player) {
		return;
	}
	int32 index = GameInstance->CurrentSlotIndex;

	GameInstance->SaveGameData_Sync(player, index);

	CloseOverwriteWidget();
	CloseSelectionWidget();

	SaveLoadSlots[index]->UpdateSlotInfo(index);
}

void UCAS_SaveLoadWidget::CloseOverwriteWidget()
{
	if (!SelectionWidget) {
		return;
	}
	OverwriteWidget->SetVisibility(ESlateVisibility::Collapsed);
	SelectionWidget->SetKeyboardFocus();
}

void UCAS_SaveLoadWidget::SaveLoadFromSlot()
{
	auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());

	auto test = GetOwningPlayer();

	auto pawn = GetOwningPlayer()->GetPawn();

	if (!pawn || !GameInstance) {
		return;
	}

	auto player = Cast<ACAS_Player>(pawn);

	//if (!player) {
	//	return;
	//}

	int32 index = SelectionWidget->GetSelectedIndex();
	GameInstance->CurrentSlotIndex = index;

	if (bSaveMode) {
		if (UGameplayStatics::DoesSaveGameExist(FString::Printf(TEXT("SLOT_%d"), index), 0)) {
			OverwriteWidget->SetVisibility(ESlateVisibility::Visible);
			OverwriteWidget->SetKeyboardFocus();
			return;
		}
		else {
			GameInstance->SaveGameData_Sync(player, index);
			CloseSelectionWidget();
		}
	}
	else {
		GameInstance->LoadGameData_ASync(index);
	}
	SaveLoadSlots[index]->UpdateSlotInfo(index);
}
