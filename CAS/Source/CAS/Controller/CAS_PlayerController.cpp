// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "UI/CAS_TitleWidget.h"
#include "Character/CAS_Player.h"


void ACAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
   

    SaveLoadWidget = CreateWidget<UCAS_SaveLoadWidget>(GetWorld(), SaveLoadWidgetClass);
    SaveLoadWidget->AddToViewport(3);
    SaveLoadWidget->CloseSaveLoadWidget();

    ExitUIMode();

    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

    if (CurrentLevelName == FName("Title")) {
        auto titleWidget = CreateWidget<UCAS_TitleWidget>(GetWorld(), TitleWidgetClass);
        TitleWidget = titleWidget;
        TitleWidget->AddToViewport(2);
        TitleWidget->SetVisibility(ESlateVisibility::Visible);
        EnterUIMode();
    }

//#if WITH_EDITOR
//    if (IsLocalController())
//    {
//        ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
//    }
//#endif

    CurrentCursorWidget = CreateWidget<UUserWidget>(this, MouseCursorClass);
    SetMouseCursorWidget(EMouseCursor::Default, CurrentCursorWidget);
    DetectingEnemy.Reserve(5);

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
        EnhancedInputComponent->BindAction(ControlSaveLoadAction, ETriggerEvent::Started, this, &ThisClass::ControlSaveLoadWidget);
        EnhancedInputComponent->BindAction(QuitGameAction, ETriggerEvent::Started, this, &ThisClass::QuitGame);
        EnhancedInputComponent->BindAction(CursorMoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveVirtualCursor);
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

        // 기존 위젯 제거
        if (CurrentCursorWidget)
        {
            CurrentCursorWidget->RemoveFromParent();
            CurrentCursorWidget = nullptr;
        }

        // 새로운 커서 위젯 지정
        if (DeviceType == EInputDeviceType::Gamepad && GamepadCursorClass)
        {
            CurrentCursorWidget = CreateWidget<UUserWidget>(this, GamepadCursorClass);
        }
        else if (DeviceType == EInputDeviceType::KeyboardMouse && MouseCursorClass)
        {
            CurrentCursorWidget = CreateWidget<UUserWidget>(this, MouseCursorClass);
        }

        if (CurrentCursorWidget)
        {
            // PlayerController의 SetMouseCursorWidget 이용
            SetMouseCursorWidget(EMouseCursor::Default, CurrentCursorWidget);
        }


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

void ACAS_PlayerController::ControlSaveLoadWidget(const FInputActionValue& Value)
{
    auto result = SaveLoadWidget->GetVisibility();
    if (result == ESlateVisibility::Visible) {
        SaveLoadWidget->CloseSaveLoadWidget();
    }
    else {
        SaveLoadWidget->DisplaySaveLoadWidget();
    }
}

void ACAS_PlayerController::QuitGame(const FInputActionValue& Value)
{
    UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void ACAS_PlayerController::EnterUIMode()
{
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ACAS_PlayerController::ExitUIMode()
{
    auto player = Cast<ACAS_Player>(GetCharacter());

    if (player->IsValidLowLevel())
    {
        if (player->IsDead())
        {
            OpenTitle();
            return;
        }
    }

    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

    if (CurrentLevelName == FName("Title"))
    {
        OpenTitle();
        return;
    }

    UGameplayStatics::SetGamePaused(GetWorld(), false);

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
}

void ACAS_PlayerController::OpenTitle()
{
    auto titleWidget = CreateWidget<UCAS_TitleWidget>(GetWorld(), TitleWidgetClass);
    TitleWidget = titleWidget;
    TitleWidget->AddToViewport(2);
    TitleWidget->SetVisibility(ESlateVisibility::Visible);
    EnterUIMode();
}


void ACAS_PlayerController::MoveVirtualCursor(const FInputActionValue& Value)
{
    if (CurrentDevice != EInputDeviceType::Gamepad) return;

    FVector2D StickValue = Value.Get<FVector2D>();
    if (!StickValue.IsNearlyZero())
    {
        FVector2D CurrentPos;
        GetMousePosition(CurrentPos.X, CurrentPos.Y);

        float Speed = 10.f;
        FVector2D NewPos = CurrentPos + StickValue * Speed;

        // PlayerController에 커서 좌표 세팅
        SetMouseLocation(NewPos.X, NewPos.Y);
    }
}

void ACAS_PlayerController::AddDetectingEnemy(class ACAS_Character* enemy)
{
    if (DetectingEnemy.Find(enemy) < 0)
        DetectingEnemy.Add(enemy);
}

void ACAS_PlayerController::RemoveDetectingEnemy(class ACAS_Character* enemy)
{
    if (!DetectingEnemy.IsEmpty())
        DetectingEnemy.Remove(enemy);
}


