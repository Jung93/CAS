// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeyPressMovementUI.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"

void UCAS_KeyPressMovementUI::NativeConstruct()
{
	Super::NativeConstruct();

	LeftWidget->WidgetClickEvent_child.AddUObject(this, &ThisClass::OnSelectionWidgetClicked);
	RightWidget->WidgetClickEvent_child.AddUObject(this, &ThisClass::OnSelectionWidgetClicked);
	
	TArray<UWidget*> FoundWidgets;
	WidgetTree->GetAllWidgets(FoundWidgets);

	for (auto Widget : FoundWidgets)
	{
		if (UCAS_MouseSelectionWidget* MouseWidget = Cast<UCAS_MouseSelectionWidget>(Widget))
		{
			Widgets.Add(MouseWidget);
		}
	}
}

void UCAS_KeyPressMovementUI::OnSelectionWidgetClicked(EWidgetPositionType Type)
{
	if (Type == EWidgetPositionType::NONE) {
		return;
	}
	WidgetClickEvent.Broadcast(Type);
}

void UCAS_KeyPressMovementUI::SetMovementTexture(UTexture2D* Texture, EWidgetPositionType Type)
{
	for (auto Widget : Widgets) {
		if (Widget->GetWidgetPositionType() == Type) {
			Widget->SetTexture(Texture);
			return;
		}
	}
}
