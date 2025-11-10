// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeySettingSlot.h"
#include "UI/CAS_KeySettingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCAS_KeySettingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	KeyButton->OnClicked.AddDynamic(this, &ThisClass::SetButtonBackgroundColor);
}

void UCAS_KeySettingSlot::SetButtonBackgroundColor()
{
	IsClicked = !IsClicked;
	FButtonStyle buttonStyle = KeyButton->WidgetStyle;
	FLinearColor color;

	if (IsClicked)
		color = FLinearColor(1, 0, 0, 1);
	else
		color = FLinearColor(1, 0, 0, 0);


	buttonStyle.Normal.OutlineSettings.Color = FSlateColor(color);

	KeyButton->SetStyle(buttonStyle);


	ClickSlot.Broadcast(this);

}

void UCAS_KeySettingSlot::InitialSetting(FName Action, UTexture2D* Icon)
{
	FText ActionText = FText::FromName(Action);
	ActionName->SetText(ActionText);

	KeyIcon->SetBrushFromTexture(Icon);
}


//void UCAS_KeySettingSlot::InitialSetting(FName Action, FName Key, UTexture2D* Icon)
//{
//	FText ActionText = FText::FromName(Action);
//	FText KeyText = FText::FromName(Key);
//
//	ActionName->SetText(ActionText);
//	KeyName->SetText(KeyText);
//
//	KeyIcon->SetBrushFromTexture(Icon);
//}
