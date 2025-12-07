// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_GamepadKeySettingSlot.h"
#include "Components/InputKeySelector.h"

void UCAS_GamepadKeySettingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	KeySelector->OnKeySelected.AddDynamic(this, &ThisClass::OnSelect);
}

void UCAS_GamepadKeySettingSlot::OnSelect(FInputChord NewKey)
{
	FKey Key = NewKey.Key;
	FName InputType = Key.GetMenuCategory();

	if (InputType.IsEqual("Key"))
		return;

	FName KeyName = Key.GetFName();

	ChangeSlot.Broadcast(this, KeyName, InputType);
}

