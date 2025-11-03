// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserSpawnActor.h"
#include "LevelManager/CAS_LaserComponent.h"

ACAS_LaserSpawnActor::ACAS_LaserSpawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);

	LaserComponent = CreateDefaultSubobject<UCAS_LaserComponent>(TEXT("LaserComponent"));
	LaserComponent->SetupAttachment(StaticMesh);
}

void ACAS_LaserSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACAS_LaserSpawnActor::SetLaserActivated(bool LaserActivated)
{
	LaserComponent->SetLaserActivated(LaserActivated);
}

bool ACAS_LaserSpawnActor::GetLaserActivated()
{
	return LaserComponent->GetLaserActivated();
}
