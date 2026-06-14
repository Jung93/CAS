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
	
	virtual void InitialSetting(bool IsSaveMode = false);

	UFUNCTION()
	void DisplaySelectionWidget();
	UFUNCTION()
	void CloseSelectionWidget();
	UFUNCTION()
	void DisplaySaveLoadWidget();
	UFUNCTION(BlueprintCallable)
	void CloseSaveLoadWidget();
	UFUNCTION()
	void OverwriteSlot();
	UFUNCTION()
	void CloseOverwriteWidget();
	UFUNCTION()
	void SaveLoadFromSlot();

	bool bSaveMode = false;

	UFUNCTION(BlueprintCallable)
	void SetPrevOpenedWidget(UUserWidget* Widget) { PrevOpenedWidget = Widget; };

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetPrevOpenedWidget() { return PrevOpenedWidget; }

	UFUNCTION(BlueprintCallable)
	void RefreshSlots();

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
	UPROPERTY()
	UCAS_SelectWidget* OverwriteWidget = nullptr;
	
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* CAS_ExitButton;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* CAS_VerticalBox;

	UPROPERTY(Visibleanywhere, BlueprintReadWrite)
	UUserWidget* PrevOpenedWidget;

};
