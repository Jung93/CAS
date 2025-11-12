// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeySettingWidget.h"
#include "UI/CAS_KeySettingSlot.h"
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

	const TArray<FEnhancedActionKeyMapping>& array = owner->GetCurrentKeyArray();


	if (KeySettingSlotWidgetClass)
	{
		for (int32 i = 0; i < array.Num(); i++)
		{
			UCAS_KeySettingSlot* slot = CreateWidget<UCAS_KeySettingSlot>(GetWorld(), KeySettingSlotWidgetClass);

			FName Action = array[i].Action.GetFName();
			FName KeyName = array[i].Key.GetFName();
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
	SetVisibility(ESlateVisibility::Collapsed);
}


//FReply UCAS_KeySettingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
//{
//	if (!KeySettingSlotWidget)
//		return FReply::Unhandled();
//
//	const FKey Key = InKeyEvent.GetKey();
//	FName KeyName = Key.GetFName();
//
//	UTexture2D* Icon = nullptr;
//
//	const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
//	if (Row)
//	{
//		Icon = Row->Icon.LoadSynchronous();
//	}
//
//
//	auto owner = Cast<ACAS_PlayerController>(GetOwningPlayer());
//
//	const TArray<FEnhancedActionKeyMapping>& array = owner->GetCurrentKeyArray();
//
//	auto Iter = array.FindByPredicate([KeyName](const FEnhancedActionKeyMapping& KeyMapping)
//		{
//			return KeyMapping.Key.GetFName().IsEqual(KeyName);
//		});
//
//	if(Iter)
//		return FReply::Unhandled();
//
//	KeySettingSlotWidget->SlotSetting(FName(), KeyName, Icon);
//
//	//UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
//	//FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
//	//FLinearColor Color = FLinearColor(1, 0, 0, 0);
//
//	//ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);
//
//	//KeyButton->SetStyle(ButtonStyle);
//
//	KeySettingSlotWidget = nullptr;
//
//	return FReply::Unhandled();
//}

//FReply UCAS_KeySettingWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	if (!KeySettingSlotWidget)
//		return FReply::Unhandled();
//
//	if(KeySettingSlotWidget->IsHovered())
//		return FReply::Unhandled();
//
//
//	const FKey Key = InMouseEvent.GetEffectingButton();
//	FName KeyName = Key.GetFName();
//
//	UTexture2D* Icon = nullptr;
//
//	const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
//	if (Row)
//	{
//		Icon = Row->Icon.LoadSynchronous();
//	}
//
//	KeySettingSlotWidget->SlotSetting(FName(), KeyName, Icon);
//
//	//UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
//	//FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
//	//FLinearColor Color = FLinearColor(1, 0, 0, 0);
//
//	//ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);
//
//	//KeyButton->SetStyle(ButtonStyle);
//
//	KeySettingSlotWidget = nullptr;
//
//	return FReply::Unhandled();
//}

void UCAS_KeySettingWidget::SetClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName CurrentKeyName)
{
	if (KeySettingSlotWidget == nullptr)
	{
		KeySettingSlotWidget = ClickedSlot;

		auto Image = KeySettingSlotWidget->GetKeyIcon();

		auto Color = Image->GetColorAndOpacity();
		Color.A = 0.7f;
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
			NewColor.A = 0.7f;
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

	FMapPlayerKeyArgs KeyArgs;


	KeyArgs.MappingName = "Jump";
	KeyArgs.NewKey = FKey("M");
	KeyArgs.Slot = EPlayerMappableKeySlot::First;

	FGameplayTagContainer tags;

	UserSetting->MapPlayerKey(KeyArgs, tags);
	UserSetting->ApplySettings();




	ClickedSlot->SlotSetting(FName(), NewKeyName, Icon);

}


