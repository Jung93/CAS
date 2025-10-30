// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserSpawnActor.h"
#include "LevelManager/CAS_LaserComponent.h"

ACAS_LaserSpawnActor::ACAS_LaserSpawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
}

void ACAS_LaserSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLaserActivated(true);
}

void ACAS_LaserSpawnActor::SetLaserActivated(bool LaserActivated)
{
	LaserComponent->SetLaserActivated(LaserActivated);
}

bool ACAS_LaserSpawnActor::GetLaserActivated()
{
	return LaserComponent->GetLaserActivated();
}
