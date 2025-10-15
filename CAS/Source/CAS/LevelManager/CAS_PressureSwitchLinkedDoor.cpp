// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_PressureSwitchLinkedDoor.h"

// Sets default values
ACAS_PressureSwitchLinkedDoor::ACAS_PressureSwitchLinkedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACAS_PressureSwitchLinkedDoor::BeginPlay()
{
	Super::BeginPlay();

	if (SwitchInstance) {
		SwitchInstance->OnSwitchPressed.AddUObject(this, &ThisClass::SwitchPressed);
	}
}

void ACAS_PressureSwitchLinkedDoor::SwitchPressed(bool SwithOn)
{
}

// Called every frame
void ACAS_PressureSwitchLinkedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

