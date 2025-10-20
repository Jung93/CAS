// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_StageDoor.h"
#include "LevelManager/CAS_WorldSubsystem.h"

// Sets default values
ACAS_StageDoor::ACAS_StageDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base__Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base_Mesh"));
	Door__Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door_Mesh"));

	SetRootComponent(Base__Mesh);
	Door__Mesh->SetupAttachment(Base__Mesh);

}

// Called when the game starts or when spawned
void ACAS_StageDoor::BeginPlay()
{
	Super::BeginPlay();
	
	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnPuzzleCompleted.AddUObject(this, &ThisClass::DoorOpen);

	DoorPosition = Door__Mesh->GetRelativeLocation();
}
void ACAS_StageDoor::DoorOpen()
{
	bDoorOpen = true;
}
void ACAS_StageDoor::DoorOpenEvent(float DeltaTime)
{
	FVector TargetLocation = DoorPosition;
	TargetLocation.Z += Offset;

	FVector CurrentLocation = Door__Mesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	Door__Mesh->SetRelativeLocation(NextLocation);
}

void ACAS_StageDoor::DoorCloseEvent(float DeltaTime)
{
	FVector TargetLocation = DoorPosition;

	FVector CurrentLocation = Door__Mesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	Door__Mesh->SetRelativeLocation(NextLocation);
}

// Called every frame
void ACAS_StageDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDoorOpen) {
		DoorOpenEvent(DeltaTime);
	}
	else {
		DoorCloseEvent(DeltaTime);
	}
}

