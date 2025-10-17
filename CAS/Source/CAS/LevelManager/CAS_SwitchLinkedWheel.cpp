// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedWheel.h"

ACAS_SwitchLinkedWheel::ACAS_SwitchLinkedWheel()
{
	PrimaryActorTick.bCanEverTick = false;

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh"));

}

void ACAS_SwitchLinkedWheel::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &ThisClass::RotateWheel, RotationInterval, true);
}
//맵 클리어 -> 타이머 클리어
void ACAS_SwitchLinkedWheel::RotateWheel()
{
	AddActorLocalRotation(FRotator(0, Degree, 0));
}
