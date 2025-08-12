// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SaveLoadSlot.h"
#include "UI/CAS_SaveLoadWidget.h"

void UCAS_SaveLoadSlot::NativeConstruct()
{
	Super::NativeConstruct();

	CAS_SaveLoadButton->OnClicked.AddDynamic(this, &ThisClass::OnClikcedEvent);
}

void UCAS_SaveLoadSlot::BindOnClickedEvent(UObject* Object, FName name)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(Object,name);
	CAS_SaveLoadButton->OnClicked.Add(Delegate);
}

void UCAS_SaveLoadSlot::OnClikcedEvent()
{
	SendSlotIndex.Broadcast(SlotIndex);
}
