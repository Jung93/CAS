// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeySettingSlot.h"
#include "UI/CAS_KeySettingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Framework/Application/NavigationConfig.h"
#include "Components/InputKeySelector.h"

void UCAS_KeySettingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	//KeyButton->OnClicked.AddDynamic(this, &ThisClass::SetButtonBackgroundColor);


	KeySelector->OnKeySelected.AddDynamic(this, &ThisClass::OnSelect);
	KeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &ThisClass::OnBeginKeyChage);

}

FName UCAS_KeySettingSlot::GetActionName()
{
	FText Text = ActionName->GetText();
	return FName(Text.ToString());
}

void UCAS_KeySettingSlot::SlotSetting(FName Action, FName KeyName, UTexture2D* Icon)
{
	if(!Action.IsNone())
	{
		FText ActionText = FText::FromName(Action);
		ActionName->SetText(ActionText);
	}

	KeyIcon->SetBrushFromTexture(Icon);

	FInputChord abc;

	abc.Key = FKey(KeyName);
	KeySelector->SetSelectedKey(abc);

}

void UCAS_KeySettingSlot::OnSelect(FInputChord abc)
{
	FKey Key = abc.Key;
	FName KeyName = Key.GetFName();

	ChangeSlot.Broadcast(this, KeyName);
}

void UCAS_KeySettingSlot::OnBeginKeyChage()
{
	CurrentKey = KeySelector->GetSelectedKey();

	FKey Key = CurrentKey.Key;
	FName KeyName = Key.GetFName();


	ClickSlot.Broadcast(this, KeyName);

}
