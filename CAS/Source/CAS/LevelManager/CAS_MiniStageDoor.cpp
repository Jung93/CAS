// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_MiniStageDoor.h"

void ACAS_MiniStageDoor::BeginPlay()
{
	AActor::BeginPlay();

	DoorPosition = Door__Mesh->GetRelativeLocation();
}
