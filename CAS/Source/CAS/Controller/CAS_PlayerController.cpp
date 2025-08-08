// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

void ACAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
    if (IsLocalController())
    {
        ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
    }
#endif


}

void ACAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(_inputMappingContext, 0);
    }
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Started, this, &ThisClass::PrintDebugMessage);

	}
}

bool ACAS_PlayerController::InputKey(const FInputKeyParams& Params)
{
    Super::InputKey(Params);

    if (Params.IsGamepad())
    {
        OnLastInputDeviceChanged(EInputDeviceType::Gamepad);
    }
    else
    {
        OnLastInputDeviceChanged(EInputDeviceType::KeyboardMouse);
    }

    return false;
}

void ACAS_PlayerController::OnLastInputDeviceChanged(EInputDeviceType DeviceType)
{
    if (CurrentDevice != DeviceType)
    {
        CurrentDevice = DeviceType;

        FString DeviceName = StaticEnum<EInputDeviceType>()->GetNameStringByValue((int64)DeviceType);

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, DeviceName);

        // 예: UI 위젯에 델리게이트 호출
        OnInputDeviceChanged.Broadcast(CurrentDevice);
    }

}

void ACAS_PlayerController::PrintDebugMessage(const FInputActionValue& Value)
{
    if (IsLocalController())
    {
        ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
    }
}
