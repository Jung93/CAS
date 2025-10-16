// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedGate.h"

ACAS_SwitchLinkedGate::ACAS_SwitchLinkedGate()
{
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StaticMesh;

}

void ACAS_SwitchLinkedGate::BeginPlay()
{
	Super::BeginPlay();


	StartPosition = GetActorLocation();
	EndPosition = StartPosition + FVector(0.f, 0.f, MoveDistance);

}

void ACAS_SwitchLinkedGate::SwitchClicked(bool SwitchOn)
{
	if (SwitchOn)
		IsOpen = true;
}

void ACAS_SwitchLinkedGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsOpen)
		return;

	FVector CurrentLocation = GetActorLocation();

	// 한 프레임 이동
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, EndPosition, DeltaTime, MoveSpeed);
	SetActorLocation(NewLocation);

	if (FVector::Dist(NewLocation, EndPosition) < 1.f)
	{
		IsOpen = false;
	}

}

