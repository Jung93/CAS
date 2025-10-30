// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_PuzzleLinkedDoor.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Global/CAS_GameInstance.h"

// Sets default values
ACAS_PuzzleLinkedDoor::ACAS_PuzzleLinkedDoor()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StaticMesh;
}

void ACAS_PuzzleLinkedDoor::BeginPlay()
{
	Super::BeginPlay();
	
	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnPuzzleCompleted.AddUObject(this, &ThisClass::DoorOpen);

	auto gi = Cast<UCAS_GameInstance>(GetGameInstance());

	if (gi && gi->IsPuzzleClear())
	{
		DoorOpen();
	}
}

void ACAS_PuzzleLinkedDoor::DoorOpen()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}


