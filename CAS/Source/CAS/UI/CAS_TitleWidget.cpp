// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_TitleWidget.h"
#include "Character/CAS_Player.h"
#include "Global/CAS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/CAS_PlayerController.h"
#include "Engine/LevelStreamingDynamic.h"

void UCAS_TitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SaveLoadWidgetClass) {
        auto controller = GetWorld()->GetFirstPlayerController();
        auto playerController = Cast<ACAS_PlayerController>(controller);
		ContinueWidget = CreateWidget<UCAS_SaveLoadWidget>(playerController, SaveLoadWidgetClass);
        ContinueWidget->InitialSetting();
	}

	CAS_ContinueSlot->BindOnClickedEvent(this, "DisplayContinueWidget");
	CAS_ContinueSlot->SetButtonText(TEXT("CONTINUE GAME"));
	CAS_NewGameSlot->BindOnClickedEvent(this, "StartNewGame");
	CAS_NewGameSlot->SetButtonText(TEXT("NEW GAME"));
    CAS_QuitGameSlot->BindOnClickedEvent(this, "QuitGame");
    CAS_QuitGameSlot->SetButtonText(TEXT("Quit Game"));
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
    auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());

    GameInstance->SetNextLevelName("Test");
    GameInstance->OpenLoadingLevel();
}

void UCAS_TitleWidget::QuitGame()
{
    auto controller = GetWorld()->GetFirstPlayerController();
    auto playerController = Cast<ACAS_PlayerController>(controller);

    UKismetSystemLibrary::QuitGame(GetWorld(), playerController, EQuitPreference::Quit, true);

}
