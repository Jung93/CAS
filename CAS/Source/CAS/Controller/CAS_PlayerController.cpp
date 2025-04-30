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
