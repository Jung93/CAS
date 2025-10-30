// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionMirror.h"
#include "LevelManager/CAS_LaserComponent.h"

ACAS_InteractionMirror::ACAS_InteractionMirror()
{
	PrimaryActorTick.bCanEverTick = false;

	LaserComponent = CreateDefaultSubobject<UCAS_LaserComponent>(TEXT("LaserComponent"));

}

void ACAS_InteractionMirror::InteractionWithPlayer()
{
	AddActorLocalRotation(FRotator(0, Degree, 0));
}

void ACAS_InteractionMirror::SetLaserActivated(bool LaserActivated)
{
	LaserComponent->SetLaserActivated(LaserActivated);
}

bool ACAS_InteractionMirror::GetLaserActivated()
{
	return LaserComponent->GetLaserActivated();
}
void ACAS_InteractionMirror::ResetMirrorTransform()
{
	SetActorTransform(InitTransform);
}

void ACAS_InteractionMirror::BeginPlay()
{
	Super::BeginPlay();

	InitTransform = GetActorTransform();
}
