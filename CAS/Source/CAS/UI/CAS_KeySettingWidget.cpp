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

void UCAS_KeySettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());

	TArray<TPair<FName, FKeyMappingRow>> array = owner->GetUserSetting()->GetCurrentKeyProfile()->GetPlayerMappingRows().Array();

	if (KeySettingSlotWidgetClass)
	{
		for (int32 i = 0; i < array.Num(); i++)
		{
			UCAS_KeySettingSlot* slot = CreateWidget<UCAS_KeySettingSlot>(GetWorld(), KeySettingSlotWidgetClass);

			auto mappingArray = array[i].Value.Mappings.Array();

			FName Action = array[i].Key;
			FName KeyName = mappingArray[0].GetCurrentKey().GetFName();
			UTexture2D* Icon = nullptr;


			const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
			if (Row)
			{
				Icon = Row->Icon.LoadSynchronous();
			}

			slot->SlotSetting(Action, KeyName, Icon);

			slot->ClickSlot.AddUObject(this, &ThisClass::SetClickedSlot);
			slot->ChangeSlot.AddUObject(this, &ThisClass::ChangeClickedSlot);
			KeySettings->AddChild(slot);

		}
	}

	ExitButton->OnClicked.AddDynamic(this, &ThisClass::CloseSettingWidget);
	ResetButton->OnClicked.AddDynamic(this, &ThisClass::ResetKeySetting);

}

void UCAS_KeySettingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UCAS_KeySettingWidget::SetKeySettingSlotWidget(UCAS_KeySettingSlot* keySettingSlot)
{
	KeySettingSlotWidget = keySettingSlot;

	return;
}

void UCAS_KeySettingWidget::CloseSettingWidget()
{
	QuitGameWidget->CloseSettingWidget();
}

void UCAS_KeySettingWidget::ResetKeySetting()
{
	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());
	auto array = owner->GetUserSetting()->GetCurrentKeyProfile()->GetPlayerMappingRows().Array();
	auto UserSetting = owner->GetUserSetting();

	for (int32 i = 0; i < array.Num(); i++)
	{
		UCAS_KeySettingSlot* slot = Cast<UCAS_KeySettingSlot>(KeySettings->GetChildAt(i));

		if (slot)
		{
			FName DefaultKey = array[i].Value.Mappings.Array()[0].GetDefaultKey().GetFName();

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

			slot->SlotSetting(FName(), DefaultKey, Icon);

			if (ActionName.IsEqual("Interaction"))
				owner->ApplyKeyToUI(Icon);

			if (ActionName.IsEqual("SlotChange1") || ActionName.IsEqual("SlotChange2"))
			{
				owner->ApplyQuickSlotKeyToUI(ActionName, Icon);
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

void UCAS_KeySettingWidget::ChangeClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName NewKeyName)
{
	UTexture2D* Icon = nullptr;

	const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(NewKeyName, "Jump");
	if (Row)
	{
		Icon = Row->Icon.LoadSynchronous();
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

	KeyArgs.MappingName = ActionName;
	KeyArgs.NewKey = FKey(NewKeyName);
	KeyArgs.Slot = EPlayerMappableKeySlot::First;

	FGameplayTagContainer tags;

	UserSetting->MapPlayerKey(KeyArgs, tags);
	UserSetting->ApplySettings();
	UserSetting->SaveSettings();

	ClickedSlot->SlotSetting(FName(), NewKeyName, Icon);

	if (ActionName.IsEqual("Interaction"))
		owner->ApplyKeyToUI(Icon);

	if (ActionName.IsEqual("SlotChange1") || ActionName.IsEqual("SlotChange2"))
	{
		owner->ApplyQuickSlotKeyToUI(ActionName, Icon);
	}
}


