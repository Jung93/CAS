// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_MouseSelectionWidget.h"

void UCAS_MouseSelectionWidget::SetTexture(UTexture2D* Texture)
{
	CAS_IconImage->SetBrushFromTexture(Texture);
}

FReply UCAS_MouseSelectionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);;
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {

		WidgetClickEvent_child.Broadcast(WidgetPositionType);
	}
	return FReply::Handled();
}

void UCAS_MouseSelectionWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CAS_Border->SetBrushColor(EnterColor);
}

void UCAS_MouseSelectionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	CAS_Border->SetBrushColor(LeaveColor);
}
