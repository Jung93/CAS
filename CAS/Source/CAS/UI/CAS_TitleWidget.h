// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/CAS_SaveLoadSlot.h"
#include "UI/CAS_SelectWidget.h"
#include "UI/CAS_SaveLoadWidget.h"
#include "CAS_TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_TitleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	//virtual void NativePreConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void DisplayContinueWidget();
	UFUNCTION(BlueprintCallable)
	void CloseContinueWidget();
	UFUNCTION(BlueprintCallable)
	void StartNewGame();
	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	UCAS_SaveLoadWidget* GetContinueWidget() { return ContinueWidget; }

private:			
	UPROPERTY(EditAnywhere, Category = "Slots")
	TSubclassOf<UCAS_SaveLoadSlot> SlotWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Slots")
	TSubclassOf<UCAS_SaveLoadWidget> SaveLoadWidgetClass;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCAS_SaveLoadSlot* CAS_ContinueSlot;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCAS_SaveLoadSlot* CAS_NewGameSlot;
	UPROPERTY(EditAnywhere)
	UCAS_SaveLoadWidget* ContinueWidget;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCAS_SaveLoadSlot* CAS_QuitGameSlot;

	FName StartLevel = "Test";

};
