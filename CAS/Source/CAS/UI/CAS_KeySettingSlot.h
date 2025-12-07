// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CAS_KeySettingSlot.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FClickSlot, UCAS_KeySettingSlot*, FName);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FChangeSlot, UCAS_KeySettingSlot*, FName, FName);


UCLASS()
class CAS_API UCAS_KeySettingSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	class UImage* GetKeyIcon() { return KeyIcon; }
	FName GetActionName();

	UFUNCTION()
	void SlotSetting(FName Action, FName KeyName, UTexture2D* Icon, int32 Index);

	//void OnSelect(FInputChord NewKey);

	UFUNCTION()
	void OnBeginKeyChage();

	UFUNCTION(BlueprintCallable)
	int32 GetSlotIndex() { return SlotIndex; }

	FClickSlot ClickSlot;
	FChangeSlot ChangeSlot;

	//virtual FReply NativeOnFocusReceived(const FGeometry& Geometry, const FFocusEvent& event) override;


protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ActionName;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* KeyIcon;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UInputKeySelector* KeySelector;

	bool IsClicked = false;

	FInputChord CurrentKey;

	int32 SlotIndex = 0;

};
