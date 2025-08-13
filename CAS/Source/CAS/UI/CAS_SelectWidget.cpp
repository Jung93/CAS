// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SelectWidget.h"

void UCAS_SelectWidget::YES_OnClickedEvent(UObject* Object, FName name)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(Object, name);
	YES_Button->OnClicked.Add(Delegate);
}

void UCAS_SelectWidget::NO_OnClickedEvent(UObject* Object, FName name)
{
	FScriptDelegate Delegate;
	Delegate.BindUFunction(Object, name);
	NO_Button->OnClicked.Add(Delegate);
}
