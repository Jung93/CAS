// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "UI/CAS_SaveLoadWidget.h"
#include "CAS_PlayerController.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChanged, EInputDeviceType);

UENUM(BlueprintType)
enum class EInputDeviceType : uint8
{
	KeyboardMouse,
	Gamepad
};


UCLASS()
class CAS_API ACAS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void EnterUIMode();
	void ExitUIMode();

	FOnInputDeviceChanged OnInputDeviceChanged;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual bool InputKey(const FInputKeyParams& Params) override;

	void OnLastInputDeviceChanged(EInputDeviceType DeviceType);
	void PrintDebugMessage(const FInputActionValue& Value);
	void ControlSaveLoadWidget(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* _inputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveLoad", meta = (AllowPrivateAccess = "true"))
	UInputAction* ControlSaveLoadAction;
	UPROPERTY()
	EInputDeviceType CurrentDevice = EInputDeviceType::KeyboardMouse;
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCAS_SaveLoadWidget> SaveLoadWidgetClass;
	UPROPERTY()
	UCAS_SaveLoadWidget* SaveLoadWidget;

	UPROPERTY()
	class UCAS_TitleWidget* TitleWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAS_TitleWidget> TitleWidgetClass;
};
