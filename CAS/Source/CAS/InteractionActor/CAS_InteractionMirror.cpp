// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionMirror.h"
#include "LevelManager/CAS_LaserTarget.h"

ACAS_InteractionMirror::ACAS_InteractionMirror()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACAS_InteractionMirror::InteractionWithPlayer()
{
	AddActorLocalRotation(FRotator(0, Degree, 0));
}

void ACAS_InteractionMirror::ResetMirrorTransform()
{
	SetActorTransform(InitTransform);
}

void ACAS_InteractionMirror::BeginPlay()
{
	Super::BeginPlay();

	SetLaserActivated(false);
}
