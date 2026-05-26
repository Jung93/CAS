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


	KeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &ThisClass::OnBeginKeyChage);

}

FName UCAS_KeySettingSlot::GetActionName()
{
	FText Text = ActionName->GetText();
	return FName(Text.ToString());
}

void UCAS_KeySettingSlot::SlotSetting(FName Action, FName KeyName, UTexture2D* Icon, int32 Index)
{
	if(!Action.IsNone())
	{
		FString aaa = Action.ToString();

		FString target = "Gamepad";
		FString to = "";

		aaa = aaa.Replace(*target, *to);

		FName newName = FName(aaa);

		FText ActionText = FText::FromName(newName);

		ActionName->SetText(ActionText);
	}

	KeyIcon->SetBrushFromTexture(Icon);

	FInputChord abc;

	abc.Key = FKey(KeyName);
	KeySelector->SetSelectedKey(abc);
	SlotIndex = Index;
}

//void UCAS_KeySettingSlot::OnSelect(FInputChord NewKey)
//{
//	FKey Key = NewKey.Key;
//	FName KeyName = Key.GetFName();
//	FName InputType = Key.GetMenuCategory();
//
//
//	ChangeSlot.Broadcast(this, KeyName);
//}
//
void UCAS_KeySettingSlot::OnBeginKeyChage()
{
	CurrentKey = KeySelector->GetSelectedKey();

	FKey Key = CurrentKey.Key;
	FName KeyName = Key.GetFName();


	ClickSlot.Broadcast(this, KeyName);

}

//FReply UCAS_KeySettingSlot::NativeOnFocusReceived(const FGeometry& Geometry, const FFocusEvent& event)
//{
//	Super::NativeOnFocusReceived(Geometry, event);
//
//
//
//
//	return FReply::Handled();
//}
