// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CAS_KeyPressUI.h"
#include "UI/CAS_MouseSelectionWidget.h"
#include "CAS_KeyPressMovementUI.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FWidgetClickEvent, EWidgetPositionType);

UCLASS()
class CAS_API UCAS_KeyPressMovementUI : public UCAS_KeyPressUI
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void OnSelectionWidgetClicked(EWidgetPositionType Type);

	void SetMovementTexture(UTexture2D* Texture , EWidgetPositionType Type);

	FWidgetClickEvent WidgetClickEvent;
	
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UCAS_MouseSelectionWidget* LeftWidget;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UCAS_MouseSelectionWidget* RightWidget;

	TArray<UCAS_MouseSelectionWidget*> Widgets;
};
