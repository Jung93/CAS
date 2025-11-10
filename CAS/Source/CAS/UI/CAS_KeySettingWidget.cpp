// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeySettingWidget.h"
#include "UI/CAS_KeySettingSlot.h"
#include "Components/VerticalBox.h"
#include "Controller/CAS_PlayerController.h"
#include "InputMappingContext.h"
#include "Data/InputKeyIconData.h"

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
			FName Key = array[i].Key.GetFName();
			UTexture2D* icon = nullptr;

			const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(Key, "Jump");
			if (Row)
			{
				icon = Row->Icon.LoadSynchronous();
			}

			slot->SlotSetting(Action, icon);
			//slot->InitialSetting(Action, Key, icon);

			slot->ClickSlot.AddUObject(this, &ThisClass::SetClickedSlot);
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


FReply UCAS_KeySettingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (!KeySettingSlotWidget)
		return FReply::Unhandled();

	const FKey Key = InKeyEvent.GetKey();
	FName KeyName = Key.GetFName();

	UTexture2D* Icon = nullptr;

	const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
	if (Row)
	{
		Icon = Row->Icon.LoadSynchronous();
	}

	KeySettingSlotWidget->SlotSetting(FName(), Icon);

	UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
	FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
	FLinearColor Color = FLinearColor(1, 0, 0, 0);

	ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);

	KeyButton->SetStyle(ButtonStyle);

	KeySettingSlotWidget = nullptr;

	return FReply::Unhandled();
}

FReply UCAS_KeySettingWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!KeySettingSlotWidget)
		return FReply::Unhandled();

	if(KeySettingSlotWidget->IsHovered())
		return FReply::Unhandled();


	const FKey Key = InMouseEvent.GetEffectingButton();
	FName KeyName = Key.GetFName();

	UTexture2D* Icon = nullptr;

	const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
	if (Row)
	{
		Icon = Row->Icon.LoadSynchronous();
	}

	KeySettingSlotWidget->SlotSetting(FName(), Icon);

	UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
	FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
	FLinearColor Color = FLinearColor(1, 0, 0, 0);

	ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);

	KeyButton->SetStyle(ButtonStyle);

	KeySettingSlotWidget = nullptr;

	return FReply::Unhandled();
}

void UCAS_KeySettingWidget::SetClickedSlot(UCAS_KeySettingSlot* ClickedSlot)
{
	if (KeySettingSlotWidget == nullptr)
	{
		KeySettingSlotWidget = ClickedSlot;

		UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
		FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
		FLinearColor Color = FLinearColor(1, 0, 0, 1);

		ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);

		KeyButton->SetStyle(ButtonStyle);

	}
	else
	{
		if (KeySettingSlotWidget == ClickedSlot)
		{
			UButton* KeyButton = KeySettingSlotWidget->GetKeyButton();
			FButtonStyle ButtonStyle = KeyButton->WidgetStyle;
			FLinearColor Color = FLinearColor(1, 0, 0, 0);

			ButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Color);

			KeyButton->SetStyle(ButtonStyle);

			KeySettingSlotWidget = nullptr;
		}
		else
		{
			UButton* OldKeyButton = KeySettingSlotWidget->GetKeyButton();
			FButtonStyle OldButtonStyle = OldKeyButton->WidgetStyle;
			FLinearColor OldColor = FLinearColor(1, 0, 0, 0);

			OldButtonStyle.Normal.OutlineSettings.Color = FSlateColor(OldColor);

			OldKeyButton->SetStyle(OldButtonStyle);

			KeySettingSlotWidget = ClickedSlot;

			UButton* NewKeyButton = KeySettingSlotWidget->GetKeyButton();
			FButtonStyle NewButtonStyle = NewKeyButton->WidgetStyle;
			FLinearColor Newcolor = FLinearColor(1, 0, 0, 1);

			NewButtonStyle.Normal.OutlineSettings.Color = FSlateColor(Newcolor);

			NewKeyButton->SetStyle(NewButtonStyle);
		}

	}


}


