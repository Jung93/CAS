// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "UI/CAS_SaveLoadWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
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
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	void EnterUIMode();
	void ExitUIMode();

	void EnterTitleUIMode();
	void ExitTitleUIMode();

	void OpenTitle();

	FOnInputDeviceChanged OnInputDeviceChanged;

	bool IsAnyDetectingEnemy() { return !DetectingEnemy.IsEmpty(); }

	void AddDetectingEnemy(class ACAS_Character* enemy);

	void RemoveDetectingEnemy(class ACAS_Character* enemy);

	TArray<class ACAS_Character*> GetEnemies() { return DetectingEnemy; }

	void ClearDetectingEnemy() { DetectingEnemy.Empty(); }


	class UCAS_TitleWidget* GetTitleWidget() {return TitleWidget;};

	class UInputMappingContext* GetUInputMappingContext() {return _inputMappingContext;}

	void DisableInputWhenAttack();
	void EnableInputWhenAttack();


	const TArray<FEnhancedActionKeyMapping>& GetCurrentKeyArray() { return CurrentMappingArray; }


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual bool InputKey(const FInputKeyParams& Params) override;

	void OnLastInputDeviceChanged(EInputDeviceType DeviceType);
	void PrintDebugMessage(const FInputActionValue& Value);
	void ControlSaveLoadWidget(const FInputActionValue& Value);
	void QuitGame(const FInputActionValue& Value);
	void MoveVirtualCursor(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* _inputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* LookOnlygMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveLoad", meta = (AllowPrivateAccess = "true"))
	UInputAction* ControlSaveLoadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuitGame", meta = (AllowPrivateAccess = "true"))
	UInputAction* QuitGameAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cursor", meta = (AllowPrivateAccess = "true"))
	UInputAction* CursorMoveAction;


	UPROPERTY()
	EInputDeviceType CurrentDevice = EInputDeviceType::KeyboardMouse;

	UPROPERTY()
	TArray<FEnhancedActionKeyMapping> CurrentMappingArray;

	UPROPERTY()
	TArray<FEnhancedActionKeyMapping> NewMappingArray;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCAS_SaveLoadWidget> SaveLoadWidgetClass;
	UPROPERTY()
	UCAS_SaveLoadWidget* SaveLoadWidget;

	UPROPERTY()
	class UCAS_TitleWidget* TitleWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAS_TitleWidget> TitleWidgetClass;


	UPROPERTY()
	class UCAS_QuitGameWidget* QuitGameWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAS_QuitGameWidget> QuitGameWidgetClass;




	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<UUserWidget> GamepadCursorClass;

	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<UUserWidget> MouseCursorClass;

	UPROPERTY()
	UUserWidget* CurrentCursorWidget;

	UPROPERTY()
	TArray<class ACAS_Character*> DetectingEnemy;

};
