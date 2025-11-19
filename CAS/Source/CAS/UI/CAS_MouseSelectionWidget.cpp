// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_MouseSelectionWidget.h"

void UCAS_MouseSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CAS_Button->OnClicked.AddDynamic(this, &ThisClass::ClickedEvent);
	CAS_Button->OnHovered.AddDynamic(this, &ThisClass::HoveredEvent);
	CAS_Button->OnUnhovered.AddDynamic(this, &ThisClass::UnhoveredEvent);
}

void UCAS_MouseSelectionWidget::SetTexture(UTexture2D* Texture)
{
	CAS_IconImage->SetBrushFromTexture(Texture);
}

void UCAS_MouseSelectionWidget::ClickedEvent()
{
	WidgetClickEvent_child.Broadcast(WidgetPositionType);
}

void UCAS_MouseSelectionWidget::HoveredEvent()
{
	CAS_IconImage->SetColorAndOpacity(EnterColor);
}

void UCAS_MouseSelectionWidget::UnhoveredEvent()
{
	CAS_IconImage->SetColorAndOpacity(LeaveColor);
}

