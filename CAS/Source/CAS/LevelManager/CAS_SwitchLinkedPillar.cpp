// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedPillar.h"
#include "CAS_SwitchLinkedPillar.h"

ACAS_SwitchLinkedPillar::ACAS_SwitchLinkedPillar()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
}

void ACAS_SwitchLinkedPillar::BeginPlay()
{
	Super::BeginPlay();

}

void ACAS_SwitchLinkedPillar::SwitchClicked(bool SwitchOn)
{	
	StartPosition = GetActorLocation();

	if (SwitchOn) {
		EndPosition = StartPosition + (GetActorUpVector() * Offset);
	}
	else {
		EndPosition = StartPosition + (GetActorUpVector() * -Offset);
	}
	GetWorldTimerManager().SetTimer(MoveHandle, this, &ACAS_SwitchLinkedPillar::SetTargetLocation, Interval, true);
}

void ACAS_SwitchLinkedPillar::SetTargetLocation()
{
	CurrDuration += Interval;

	float Alpha = FMath::Clamp(CurrDuration / MaxDuration, 0.0f, 1.0f);

	FVector TargetPosition = FMath::Lerp(StartPosition, EndPosition, Alpha);
	SetActorLocation(TargetPosition);

	if (Alpha >= 1.0f)
	{
		CurrDuration = 0.0f;
		GetWorldTimerManager().ClearTimer(MoveHandle);
	}
}
