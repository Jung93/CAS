// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/CAS_SelectWidget.h"

#include "CAS_QuitGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_QuitGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseWidget();
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void OpenWidget();

private:
	UPROPERTY(EditAnywhere, Category = "Slots")
	TSubclassOf<UCAS_SelectWidget> SelectWidgetClass;

	UPROPERTY()
	UCAS_SelectWidget* SelectWidget = nullptr;
};
