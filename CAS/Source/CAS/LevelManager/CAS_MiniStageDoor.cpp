// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_MiniStageDoor.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Global/CAS_GameInstance.h"

void ACAS_MiniStageDoor::BeginPlay()
{
	AActor::BeginPlay();

	DoorPosition = Door__Mesh->GetRelativeLocation();

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnPuzzleCompleted.AddUObject(this, &ThisClass::DoorOpen);

	auto gi = Cast<UCAS_GameInstance>(GetGameInstance());

	if (gi && gi->IsPuzzleClear())
	{
		DoorOpen();
	}

}
