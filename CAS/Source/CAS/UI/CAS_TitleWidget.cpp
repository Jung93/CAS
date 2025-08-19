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
        auto controller = GetWorld()->GetFirstPlayerController();
        auto playerController = Cast<ACAS_PlayerController>(controller);
		ContinueWidget = CreateWidget<UCAS_SaveLoadWidget>(playerController, SaveLoadWidgetClass);
		ContinueWidget->bSaveMode = false;	
        ContinueWidget->AddToViewport(3);
        ContinueWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	CAS_ContinueSlot->BindOnClickedEvent(this, "DisplayContinueWidget");
	CAS_ContinueSlot->SetButtonText(TEXT("CONTINUE GAME"));
	CAS_NewGameSlot->BindOnClickedEvent(this, "StartNewGame");
	CAS_NewGameSlot->SetButtonText(TEXT("NEW GAME"));
}

void UCAS_TitleWidget::DisplayContinueWidget()
{
    if (ContinueWidget)
    {
        ContinueWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void UCAS_TitleWidget::CloseContinueWidget()
{
    if (ContinueWidget)
    {
        ContinueWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UCAS_TitleWidget::StartNewGame()
{
    UWorld* World = GetWorld();
    if (World)
    {
        if (ContinueWidget)
        {
            ContinueWidget->RemoveFromParent();
            ContinueWidget = nullptr;
        }
        RemoveFromParent();
        UGameplayStatics::OpenLevel(World, StartLevel);
    }
}