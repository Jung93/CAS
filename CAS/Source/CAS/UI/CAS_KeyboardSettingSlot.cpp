// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeyboardSettingSlot.h"
#include "Components/InputKeySelector.h"

void UCAS_KeyboardSettingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	KeySelector->OnKeySelected.AddDynamic(this, &ThisClass::OnSelect);
}

void UCAS_KeyboardSettingSlot::OnSelect(FInputChord NewKey)
{
	FKey Key = NewKey.Key;
	FName InputType = Key.GetMenuCategory();

	if (InputType.IsEqual("Gamepad"))
		return;

	FName KeyName = Key.GetFName();

	ChangeSlot.Broadcast(this, KeyName, InputType);
}

