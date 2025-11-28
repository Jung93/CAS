// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/CAS_SelectWidget.h"
#include "UI/CAS_KeySettingWidget.h"
#include "UI/CAS_KeySettingSlot.h"
#include "Components/Button.h"
#include "UI/CAS_SaveLoadWidget.h"

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
	virtual void NativePreConstruct() override;


	UFUNCTION()
	void OpenWidget();
	UFUNCTION()
	void CloseWidget();

	UFUNCTION()
	void OpenSettingWidget();
	UFUNCTION()
	void CloseSettingWidget();

	UFUNCTION()
	void OpenQuitWidget();
	UFUNCTION()
	void CloseQuitWidget();

	UFUNCTION()
	void OpenLoadWidget();


	UFUNCTION()
	void QuitGame();

	UCAS_KeySettingWidget* GetKeySettingWidget() { return KeySettingWidget; }

protected:
	UPROPERTY(EditAnywhere, Category = "Slots")
	TSubclassOf<UCAS_SelectWidget> QuitWidgetClass;

	UPROPERTY()
	UCAS_SelectWidget* QuitWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCAS_KeySettingWidget> KeySettingWidgetClass;

	UPROPERTY()
	UCAS_KeySettingWidget* KeySettingWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCAS_SaveLoadWidget> LoadWidgetClass;

	UPROPERTY()
	UCAS_SaveLoadWidget* LoadWidget = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	//TSubclassOf<UCAS_KeySettingSlot> KeySettingSlotWidgetClass;

	//UPROPERTY()
	//UCAS_KeySettingSlot* KeySettingSlotWidget = nullptr;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* LoadButton;
};
