// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuitGameWidget.h"
#include "UI/CAS_KeySettingWidget.h"
#include "UI/CAS_KeySettingSlot.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/CAS_PlayerController.h"
#include "UI/CAS_SaveLoadWidget.h"


void UCAS_QuitGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuitWidget = CreateWidget<UCAS_SelectWidget>(GetWorld(), QuitWidgetClass);
	KeySettingWidget = CreateWidget<UCAS_KeySettingWidget>(GetWorld(), KeySettingWidgetClass);

	if (QuitWidget) {
		QuitWidget->AddToViewport(5);
		QuitWidget->SetVisibility(ESlateVisibility::Collapsed);
		QuitWidget->NO_OnClickedEvent(this, FName("CloseQuitWidget"));
		QuitWidget->YES_OnClickedEvent(this, FName("QuitGame"));

	}

	if (KeySettingWidget) {
		KeySettingWidget->AddToViewport(5);
		KeySettingWidget->SetVisibility(ESlateVisibility::Collapsed);
		KeySettingWidget->SetQuitGameWidget(this);
	}


	if (LoadWidgetClass) {
		auto controller = GetWorld()->GetFirstPlayerController();
		auto playerController = Cast<ACAS_PlayerController>(controller);
		LoadWidget = CreateWidget<UCAS_SaveLoadWidget>(playerController, LoadWidgetClass);
		LoadWidget->InitialSetting();
	}


	SettingButton->OnClicked.AddDynamic(this, &ThisClass::OpenSettingWidget);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OpenQuitWidget);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseWidget);
	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OpenLoadWidget);
	SetVisibility(ESlateVisibility::Collapsed);

}

void UCAS_QuitGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}


void UCAS_QuitGameWidget::OpenWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->EnterUIMode();
}

void UCAS_QuitGameWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	auto controller = GetWorld()->GetFirstPlayerController();
	auto playerController = Cast<ACAS_PlayerController>(controller);
	playerController->ExitUIMode();
}

void UCAS_QuitGameWidget::OpenSettingWidget()
{
	KeySettingWidget->SetVisibility(ESlateVisibility::Visible);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCAS_QuitGameWidget::CloseSettingWidget()
{
	KeySettingWidget->SetVisibility(ESlateVisibility::Collapsed);
	SetVisibility(ESlateVisibility::Visible);
}

void UCAS_QuitGameWidget::OpenQuitWidget()
{
	QuitWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCAS_QuitGameWidget::CloseQuitWidget()
{
	QuitWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UCAS_QuitGameWidget::OpenLoadWidget()
{
	LoadWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCAS_QuitGameWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

