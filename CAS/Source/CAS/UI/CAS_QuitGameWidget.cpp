// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuitGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/CAS_PlayerController.h"

void UCAS_QuitGameWidget::NativeConstruct()
{

	SelectWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), SelectWidgetClass);

	if (SelectWidget) {
		SelectWidget->AddToViewport(5);

		SelectWidget->SetVisibility(ESlateVisibility::Collapsed);

		SelectWidget->NO_OnClickedEvent(this, FName("CloseWidget"));
		SelectWidget->YES_OnClickedEvent(this, FName("QuitGame"));

	}


}

void UCAS_QuitGameWidget::CloseWidget()
{
	SelectWidget->SetVisibility(ESlateVisibility::Collapsed);
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->ExitUIMode();
}

void UCAS_QuitGameWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UCAS_QuitGameWidget::OpenWidget()
{
	SelectWidget->SetVisibility(ESlateVisibility::Visible);
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->EnterUIMode();
}
