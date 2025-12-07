// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeySettingWidget.h"
#include "UI/CAS_KeySettingSlot.h"
#include "UI/CAS_QuitGameWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Controller/CAS_PlayerController.h"
#include "InputMappingContext.h"
#include "Data/InputKeyIconData.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Components/WidgetSwitcher.h"

void UCAS_KeySettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());

	TArray<TPair<FName, FKeyMappingRow>> array = owner->GetUserSetting()->GetCurrentKeyProfile()->GetPlayerMappingRows().Array();

	if (KeySettingSlotWidgetClass && GamepadKeySettingSlotWidgetClass)
	{
		int32 KeyboardIndex = 0;
		int32 GamepadIndex = 0;

		for (int32 i = 0; i < array.Num(); i++)
		{

			auto mappingArray = array[i].Value.Mappings.Array();

			FName Action = array[i].Key;
			FName KeyName = mappingArray[0].GetCurrentKey().GetFName();
			UTexture2D* Icon = nullptr;

			if (Action.ToString().StartsWith("Gamepad"))
			{
				UCAS_GamepadKeySettingSlot* slot = CreateWidget<UCAS_GamepadKeySettingSlot>(GetWorld(), GamepadKeySettingSlotWidgetClass);

				const FInputKeyIconData* Row = GamepadIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
				if (Row)
				{
					Icon = Row->Icon.LoadSynchronous();
				}

				KeyboardIndex = i - GamepadIndex;

				slot->SlotSetting(Action, KeyName, Icon, KeyboardIndex);

				KeyboardIndex++;

				slot->ClickSlot.AddUObject(this, &ThisClass::SetClickedSlot);
				slot->ChangeSlot.AddUObject(this, &ThisClass::ChangeClickedSlot);
				GamepadSettings->AddChild(slot);

			}
			else
			{
				UCAS_KeyboardSettingSlot* slot = CreateWidget<UCAS_KeyboardSettingSlot>(GetWorld(), KeySettingSlotWidgetClass);

				const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
				if (Row)
				{
					Icon = Row->Icon.LoadSynchronous();
				}

				GamepadIndex = i - KeyboardIndex;

				slot->SlotSetting(Action, KeyName, Icon, GamepadIndex);

				GamepadIndex++;

				slot->ClickSlot.AddUObject(this, &ThisClass::SetClickedSlot);
				slot->ChangeSlot.AddUObject(this, &ThisClass::ChangeClickedSlot);
				KeySettings->AddChild(slot);
			}


			//UCAS_KeySettingSlot* slot = CreateWidget<UCAS_KeySettingSlot>(GetWorld(), KeySettingSlotWidgetClass);

			//const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
			//if (Row)
			//{
			//	Icon = Row->Icon.LoadSynchronous();
			//}

			//slot->SlotSetting(Action, KeyName, Icon);

			//slot->ClickSlot.AddUObject(this, &ThisClass::SetClickedSlot);
			//slot->ChangeSlot.AddUObject(this, &ThisClass::ChangeClickedSlot);
			//KeySettings->AddChild(slot);

		}
	}

	ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseSettingWidget);
	ResetButton->OnClicked.AddDynamic(this, &ThisClass::ResetKeySetting);

}

void UCAS_KeySettingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

//void UCAS_KeySettingWidget::SetKeySettingSlotWidget(UCAS_KeySettingSlot* keySettingSlot)
//{
//	//KeySettingSlotWidget = keySettingSlot;
//
//	return;
//}

void UCAS_KeySettingWidget::CloseSettingWidget()
{
	QuitGameWidget->CloseSettingWidget();
}

void UCAS_KeySettingWidget::ResetKeySetting()
{
	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());
	auto UserSetting = owner->GetUserSetting();
	TArray<TPair<FName, FKeyMappingRow>> array;
	auto KeyMap = UserSetting->GetCurrentKeyProfile()->GetPlayerMappingRows();

	if (SettingSwitcher->ActiveWidgetIndex == 0) 
	{
		for (auto& Pair : KeyMap)
		{
			if (Pair.Key.ToString().StartsWith("Gamepad"))
				continue;

			array.Add(Pair);
		}

		for (int32 i = 0; i < array.Num(); i++)
		{
			UCAS_KeySettingSlot* slot = Cast<UCAS_KeySettingSlot>(KeySettings->GetChildAt(i));
			FName DefaultKey = array[i].Value.Mappings.Array()[0].GetDefaultKey().GetFName();
			int32 Index = slot->GetSlotIndex();
			if (slot)
			{
				UTexture2D* Icon = nullptr;

				const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(DefaultKey, "Jump");
				if (Row)
				{
					Icon = Row->Icon.LoadSynchronous();
				}

				FMapPlayerKeyArgs KeyArgs;
				FName ActionName = slot->GetActionName();

				KeyArgs.MappingName = ActionName;
				KeyArgs.NewKey = FKey(DefaultKey);
				KeyArgs.Slot = EPlayerMappableKeySlot::First;

				FGameplayTagContainer tags;

				UserSetting->MapPlayerKey(KeyArgs, tags);
				UserSetting->ApplySettings();
				UserSetting->SaveSettings();

				slot->SlotSetting(FName(), DefaultKey, Icon, Index);

				if (ActionName.IsEqual("Interaction"))
					owner->ApplyKeyToUI(Icon);

				if (ActionName.IsEqual("SlotChange1") || ActionName.IsEqual("SlotChange2"))
				{
					owner->ApplyQuickSlotKeyToUI(ActionName, Icon);
				}
			}

		}
	}
	else
	{
		for (auto& Pair : KeyMap)
		{
			if (Pair.Key.ToString().StartsWith("Gamepad"))
				array.Add(Pair);
		}

		for (int32 i = 0; i < array.Num(); i++)
		{
			UCAS_KeySettingSlot* slot = Cast<UCAS_KeySettingSlot>(GamepadSettings->GetChildAt(i));
			FName DefaultKey = array[i].Value.Mappings.Array()[0].GetDefaultKey().GetFName();
			int32 Index = slot->GetSlotIndex();

			if (slot)
			{
				UTexture2D* Icon = nullptr;

				const FInputKeyIconData* Row = GamepadIconTable->FindRow<FInputKeyIconData>(DefaultKey, "Jump");
				if (Row)
				{
					Icon = Row->Icon.LoadSynchronous();
				}

				FMapPlayerKeyArgs KeyArgs;
				FName ActionName = slot->GetActionName();

				KeyArgs.MappingName = ActionName;
				KeyArgs.NewKey = FKey(DefaultKey);
				KeyArgs.Slot = EPlayerMappableKeySlot::First;

				FGameplayTagContainer tags;

				UserSetting->MapPlayerKey(KeyArgs, tags);
				UserSetting->ApplySettings();
				UserSetting->SaveSettings();

				slot->SlotSetting(FName(), DefaultKey, Icon, Index);

				if (ActionName.IsEqual("Interaction"))
					owner->ApplyKeyToUI(Icon);

				if (ActionName.IsEqual("SlotChange1") || ActionName.IsEqual("SlotChange2"))
				{
					owner->ApplyQuickSlotKeyToUI(ActionName, Icon);
				}
			}

		}
	}

}


void UCAS_KeySettingWidget::SetClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName CurrentKeyName)
{
	if (KeySettingSlotWidget == nullptr)
	{
		KeySettingSlotWidget = ClickedSlot;

		auto Image = KeySettingSlotWidget->GetKeyIcon();

		auto Color = Image->GetColorAndOpacity();
		Color.A = 0.5f;
		Image->SetColorAndOpacity(Color);

	}
	else
	{
		if (KeySettingSlotWidget == ClickedSlot)
		{
			auto Image = KeySettingSlotWidget->GetKeyIcon();
			auto Color = Image->GetColorAndOpacity();
			Color.A = 1.f;
			Image->SetColorAndOpacity(Color);

			KeySettingSlotWidget = nullptr;
		}
		else
		{
			auto Image = KeySettingSlotWidget->GetKeyIcon();
			auto Color = Image->GetColorAndOpacity();
			Color.A = 1.f;
			Image->SetColorAndOpacity(Color);


			KeySettingSlotWidget = ClickedSlot;


			auto NewImage = KeySettingSlotWidget->GetKeyIcon();
			auto NewColor = NewImage->GetColorAndOpacity();
			NewColor.A = 0.5f;
			NewImage->SetColorAndOpacity(NewColor);
		}

	}


}

void UCAS_KeySettingWidget::ChangeClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName NewKeyName, FName InputType)
{
	UTexture2D* Icon = nullptr;

	if (InputType.IsEqual("Key"))
	{
		const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(NewKeyName, "Jump");
		if (Row)
		{
			Icon = Row->Icon.LoadSynchronous();
		}

	}
	else
	{
		const FInputKeyIconData* Row = GamepadIconTable->FindRow<FInputKeyIconData>(NewKeyName, "Jump");
		if (Row)
		{
			Icon = Row->Icon.LoadSynchronous();
		}
	}




	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());
	auto UserSetting = owner->GetUserSetting();

	auto MapArray = UserSetting->GetCurrentKeyProfile()->GetPlayerMappingRows().Array();

	for (auto a : MapArray)
	{
		auto arr = a.Value.Mappings.Array();

		if (arr[0].GetCurrentKey().GetFName().IsEqual(NewKeyName))
		{
			return;
		}
	}


	FMapPlayerKeyArgs KeyArgs;
	FName ActionName = ClickedSlot->GetActionName();
	int32 Index = ClickedSlot->GetSlotIndex();

	KeyArgs.MappingName = ActionName;
	KeyArgs.NewKey = FKey(NewKeyName);
	KeyArgs.Slot = EPlayerMappableKeySlot::First;

	FGameplayTagContainer tags;

	UserSetting->MapPlayerKey(KeyArgs, tags);
	UserSetting->ApplySettings();
	UserSetting->SaveSettings();

	ClickedSlot->SlotSetting(FName(), NewKeyName, Icon, Index);

	if (ActionName.IsEqual("Interaction"))
		owner->ApplyKeyToUI(Icon);

	if (ActionName.IsEqual("SlotChange1") || ActionName.IsEqual("SlotChange2"))
	{
		owner->ApplyQuickSlotKeyToUI(ActionName, Icon);
	}
}

void UCAS_KeySettingWidget::SetSlotFocus()
{
	//auto slot = Cast<UCAS_KeyboardSettingSlot>(KeySettings->GetChildAt(0));
	//slot->SetFocus();

	SetKeyboardFocus();
}


