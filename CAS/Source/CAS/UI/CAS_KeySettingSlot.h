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

DECLARE_MULTICAST_DELEGATE_OneParam(FClickSlot, UCAS_KeySettingSlot*);


UCLASS()
class CAS_API UCAS_KeySettingSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetButtonBackgroundColor();

	UFUNCTION()
	void SlotSetting(FName Action, UTexture2D* Icon);
	//void InitialSetting(FName Action, FName Key, UTexture2D* Icon);


	UButton* GetKeyButton() { return KeyButton; }

	FClickSlot ClickSlot;

protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ActionName;

	//UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	//UTextBlock* KeyName;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* KeyButton;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* KeyIcon;

	bool IsClicked = false;

};
