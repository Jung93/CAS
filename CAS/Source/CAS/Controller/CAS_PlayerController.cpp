// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_PlayerController.h"
#include "Engine/LocalPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "UI/CAS_TitleWidget.h"
#include "UI/CAS_QuitGameWidget.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"
#include "InputMappingContext.h"
//#include "Framework/Application/NavigationConfig.h"
#include "UserSettings/EnhancedInputUserSettings.h"


void ACAS_PlayerController::DisableInputWhenAttack()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->RemoveMappingContext(_inputMappingContext);
        Subsystem->AddMappingContext(LookOnlygMappingContext, 0);
    }
}

void ACAS_PlayerController::EnableInputWhenAttack()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->RemoveMappingContext(LookOnlygMappingContext);
        Subsystem->AddMappingContext(_inputMappingContext, 0);
    }


}

void ACAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

    SaveLoadWidget = CreateWidget<UCAS_SaveLoadWidget>(GetWorld(), SaveLoadWidgetClass);
    SaveLoadWidget->AddToViewport(3);
    SaveLoadWidget->CloseSaveLoadWidget();

    //ExitUIMode();
    // 
    // 
    auto MappingArray = _inputMappingContext->GetMappings();

    for (auto input : MappingArray)
    {
        if (input.Action.GetFName() == "IA_Look" ||
            input.Action.GetFName() == "IA_DebugMessage" ||
            input.Action.GetFName() == "IA_ShowMouse" ||
            input.Action.GetFName() == "IA_ControlSlot" ||
            input.Action.GetFName() == "IA_ControlSaveLoad" ||
            input.Action.GetFName() == "IA_ControllerCursor"
            )
        {
            continue;
        }
        
        CurrentMappingArray.Add(input);
    }


    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

    if (CurrentLevelName == FName("Title")) {
        auto titleWidget = CreateWidget<UCAS_TitleWidget>(GetWorld(), TitleWidgetClass);
        TitleWidget = titleWidget;
        TitleWidget->AddToViewport(2);
        TitleWidget->SetVisibility(ESlateVisibility::Visible);
        EnterTitleUIMode();
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

    QuitGameWidget = CreateWidget<UCAS_QuitGameWidget>(GetWorld(), QuitGameWidgetClass);
    QuitGameWidget->AddToViewport(4);


}

void ACAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(_inputMappingContext, 0);
        Subsystem->AddMappingContext(ControllerMappingContext, 0);

        UserSetting = Subsystem->GetUserSettings();


        if (!UserSetting->IsMappingContextRegistered(ControllerMappingContext))
        {
            UserSetting->RegisterInputMappingContext(ControllerMappingContext);
        }

        if (!UserSetting->IsMappingContextRegistered(_inputMappingContext))
        {
            UserSetting->RegisterInputMappingContext(_inputMappingContext);
        }


    }
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Started, this, &ThisClass::PrintDebugMessage);
        EnhancedInputComponent->BindAction(ControlSaveLoadAction, ETriggerEvent::Started, this, &ThisClass::ControlSaveLoadWidget);
        EnhancedInputComponent->BindAction(QuitGameAction, ETriggerEvent::Started, this, &ThisClass::QuitGame);
        //EnhancedInputComponent->BindAction(CursorMoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveVirtualCursor);

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
    //UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);

    QuitGameWidget->OpenWidget();
}

void ACAS_PlayerController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);

    auto EnemyCharacter = Cast<ACAS_EnemyCapt>(pawn);

    if (!EnemyCharacter) {
        return;
    }

    auto ASC = EnemyCharacter->GetAbilitySystemComponent();

    if (!ASC) {
        return;
    }

    ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
}

void ACAS_PlayerController::OnUnPossess()
{
    Super::OnUnPossess();
    
    auto EnemyCharacter = Cast<ACAS_EnemyCapt>(GetPawn());
   
    if (!EnemyCharacter) {
        return;
    }

    auto ASC = EnemyCharacter->GetAbilitySystemComponent();

    if (!ASC) {
        return;
    }

    ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));

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
    //auto player = Cast<ACAS_Player>(GetCharacter());

    //if (player->IsValidLowLevel())
    //{
    //    if (player->IsDead())
    //    {
    //        OpenTitle();
    //        return;
    //    }
    //}

    //FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

    //if (CurrentLevelName == FName("Title"))
    //{
    //    OpenTitle();
    //    return;
    //}

    UGameplayStatics::SetGamePaused(GetWorld(), false);

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
}

void ACAS_PlayerController::EnterTitleUIMode()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->RemoveMappingContext(_inputMappingContext);
        Subsystem->RemoveMappingContext(ControllerMappingContext);
    }

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ACAS_PlayerController::ExitTitleUIMode()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(_inputMappingContext, 0);
        Subsystem->AddMappingContext(ControllerMappingContext, 0);
    }

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


//void ACAS_PlayerController::MoveVirtualCursor(const FInputActionValue& Value)
//{
//    if (CurrentDevice != EInputDeviceType::Gamepad) return;
//
//    FVector2D StickValue = Value.Get<FVector2D>();
//    if (!StickValue.IsNearlyZero())
//    {
//        FVector2D CurrentPos;
//        GetMousePosition(CurrentPos.X, CurrentPos.Y);
//
//        float Speed = 10.f;
//        FVector2D NewPos = CurrentPos + StickValue * Speed;
//
//        // PlayerController에 커서 좌표 세팅
//        SetMouseLocation(NewPos.X, NewPos.Y);
//    }
//
//}

void ACAS_PlayerController::ApplyKeyToUI(UTexture2D* Texture)
{
    ChageUITexture.Broadcast(Texture);
}

void ACAS_PlayerController::ApplyQuickSlotKeyToUI(FName ActionName, UTexture2D* Texture)
{
    ChageQuickSlotTexture.Broadcast(ActionName, Texture);
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


