// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadSlot.h"
#include "UI/CAS_SaveLoadWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UCAS_SaveLoadSlot::NativeConstruct()
{
	Super::NativeConstruct();

	CAS_SaveLoadButton->OnClicked.Clear();
	CAS_SaveLoadButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedEvent);
	
	FString FormattedText = FString::Printf(TEXT("Slot Num : %d"), SlotIndex);
	SetButtonText(FText::FromString(FormattedText));
}

void UCAS_SaveLoadSlot::BindOnClickedEvent(UObject* Object, FName name)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(Object,name);
	CAS_SaveLoadButton->OnClicked.Add(Delegate);
}

void UCAS_SaveLoadSlot::SetButtonText(FText text)
{
	CAS_TextBlock->SetText(text);
}

void UCAS_SaveLoadSlot::SetButtonText(FString string)
{
	FText text = FText::FromString(string);
	CAS_TextBlock->SetText(text);
}

void UCAS_SaveLoadSlot::OnClickedEvent()
{
	SendSlotIndex.Broadcast(SlotIndex);
	
}
