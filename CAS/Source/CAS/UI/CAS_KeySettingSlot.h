// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CAS_KeySettingSlot.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FClickSlot, UCAS_KeySettingSlot*, FName);
DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeSlot, UCAS_KeySettingSlot*, FName);


UCLASS()
class CAS_API UCAS_KeySettingSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetButtonBackgroundColor();

	class UImage* GetKeyIcon() { return KeyIcon; }
	FName GetActionName();

	UFUNCTION()
	//void SlotSetting(FName Action, UTexture2D* Icon);
	void SlotSetting(FName Action, FName KeyName, UTexture2D* Icon);

	UFUNCTION()
	void TestSelect(FInputChord abc);

	UFUNCTION()
	void OnBeginKeyChage();

	//UButton* GetKeyButton() { return KeyButton; }

	FClickSlot ClickSlot;
	FChangeSlot ChangeSlot;



protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ActionName;

	//UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	//UTextBlock* KeyName;

	//UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	//UButton* KeyButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* KeyIcon;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UInputKeySelector* KeySelector;

	bool IsClicked = false;

	FInputChord CurrentKey;



};
