// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_TitleWidget.h"
#include "Character/CAS_Player.h"
#include "Global/CAS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/CAS_PlayerController.h"

void UCAS_TitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SaveLoadWidgetClass) {
		ContinueWidget = CreateWidget<UCAS_SaveLoadWidget>(GetWorld(), SaveLoadWidgetClass);
		ContinueWidget->bSaveMode = false;		
	}

	CAS_ContinueSlot->BindOnClickedEvent(this, "DisplayContinueWidget");
	CAS_ContinueSlot->SetButtonText(TEXT("CONTINUE GAME"));
	CAS_NewGameSlot->BindOnClickedEvent(this, "StartNewGame");
	CAS_NewGameSlot->SetButtonText(TEXT("NEW GAME"));
}

void UCAS_TitleWidget::DisplayContinueWidget()
{	
	ContinueWidget->DisplaySaveLoadWidget();
	ContinueWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCAS_TitleWidget::CloseContinueWidget()
{
	ContinueWidget->SetVisibility(ESlateVisibility::Collapsed);
}
void UCAS_TitleWidget::StartNewGame()
{
	ContinueWidget->CloseSaveLoadWidget();
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, StartLevel); 
	}
}