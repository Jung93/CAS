// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "UI/CAS_KeySettingSlot.h"
#include "CAS_KeySettingWidget.generated.h"

/**
 * 
 */



UCLASS()
class CAS_API UCAS_KeySettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	class UVerticalBox* GetVerticalBox() { return KeySettings; }
	void SetKeySettingSlotWidget(UCAS_KeySettingSlot* keySettingSlot);
	void SetQuitGameWidget(class UCAS_QuitGameWidget* Target) { QuitGameWidget = Target; };

	UFUNCTION()
	void CloseSettingWidget();

	UFUNCTION()
	void ResetKeySetting();


	UFUNCTION()
	void SetClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName CurrentKeyName);

	UFUNCTION()
	void ChangeClickedSlot(UCAS_KeySettingSlot* ClickedSlot, FName NewKeyName);


	UDataTable* GetKeyIconTable() { return KeyIconTable; };


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	TSubclassOf<UCAS_KeySettingSlot> KeySettingSlotWidgetClass;

	UPROPERTY()
	UCAS_KeySettingSlot* KeySettingSlotWidget = nullptr;

	UPROPERTY()
	class UCAS_QuitGameWidget* QuitGameWidget = nullptr;

	UPROPERTY()
	TArray<UCAS_KeySettingSlot*> KeySlots;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* KeySettings;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ResetButton;

	UPROPERTY(EditAnywhere)
	UDataTable* KeyIconTable;

};
