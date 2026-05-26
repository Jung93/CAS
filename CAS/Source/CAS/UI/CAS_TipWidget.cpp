// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_TipWidget.h"

void UCAS_TipWidget::NativeConstruct()
{

	CAS_ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseWidget);

}

void UCAS_TipWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
