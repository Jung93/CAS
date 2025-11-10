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

			slot->InitialSetting(Action, icon);
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

void UCAS_KeySettingWidget::ChangeKeySetting()
{
}

void UCAS_KeySettingWidget::SetClickedSlot(UCAS_KeySettingSlot* ClickedSlot)
{
	KeySettingSlotWidget = ClickedSlot;
}


