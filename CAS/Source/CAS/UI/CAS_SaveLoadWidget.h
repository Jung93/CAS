// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/CAS_SaveLoadSlot.h"
#include "UI/CAS_SelectWidget.h"
#include "CAS_SaveLoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SaveLoadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void DisplaySelectionWidget();
	UFUNCTION()
	void CloseSelectionWidget();
	UFUNCTION()
	void DisplaySaveLoadWidget();
	UFUNCTION()
	void CloseSaveLoadWidget();
	UFUNCTION()
	void SaveLoadFromSlot();

	bool bSaveMode = false;
protected:
	UPROPERTY(EditAnywhere, Category = "Slots")
	int32 SlotCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UCAS_SaveLoadSlot> SlotWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UCAS_SelectWidget> SelectWidgetClass;

	UPROPERTY()
	TArray<UCAS_SaveLoadSlot*> SaveLoadSlots;
	
	UPROPERTY()
	UCAS_SelectWidget* SelectionWidget = nullptr;
	
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* CAS_ExitButton;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* CAS_VerticalBox;

};
