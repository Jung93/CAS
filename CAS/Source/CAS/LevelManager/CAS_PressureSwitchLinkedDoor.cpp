// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_PressureSwitchLinkedDoor.h"

// Sets default values
ACAS_PressureSwitchLinkedDoor::ACAS_PressureSwitchLinkedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));

	SetRootComponent(BaseMesh);
	DoorMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ACAS_PressureSwitchLinkedDoor::BeginPlay()
{
	Super::BeginPlay();

	if (SwitchInstance) {
		SwitchInstance->OnSwitchPressed.AddUObject(this, &ThisClass::SwitchPressed);
	}
	
	DoorPosition = DoorMesh->GetRelativeLocation();
}

void ACAS_PressureSwitchLinkedDoor::SwitchPressed(bool SwitchOn)
{
	bDoorOpen = SwitchOn;
}

void ACAS_PressureSwitchLinkedDoor::DoorOpenEvent(float DeltaTime)
{
	FVector TargetLocation = DoorPosition;
	TargetLocation.Z += Offset;

	FVector CurrentLocation = DoorMesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	DoorMesh->SetRelativeLocation(NextLocation);
}

void ACAS_PressureSwitchLinkedDoor::DoorCloseEvent(float DeltaTime)
{
	FVector TargetLocation = DoorPosition;

	FVector CurrentLocation = DoorMesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	DoorMesh->SetRelativeLocation(NextLocation);
}

// Called every frame
void ACAS_PressureSwitchLinkedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDoorOpen) {
		DoorOpenEvent(DeltaTime);
	}
	else {
		DoorCloseEvent(DeltaTime);
	}
}

