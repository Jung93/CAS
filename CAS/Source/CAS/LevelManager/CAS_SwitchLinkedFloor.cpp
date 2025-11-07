// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedFloor.h"

ACAS_SwitchLinkedFloor::ACAS_SwitchLinkedFloor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));

}

void ACAS_SwitchLinkedFloor::SwitchClicked(bool SwitchOn)
{
	bSwitchOn = SwitchOn;
}

void ACAS_SwitchLinkedFloor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACAS_SwitchLinkedFloor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	int32 ActorNum = Actors.Num();

	if (ActorNum == 0) { 
		return;
	}

	ActorLocation.SetNum(ActorNum);

	if (FMath::IsNearlyZero(Radius)) {
		Radius = FloorMesh->Bounds.BoxExtent.X * Param;
	}

	for (int32 i = 0; i < ActorNum; i++)
	{
		float Angle = i * (360.0f / ActorNum);
		float RadianAngle = FMath::DegreesToRadians(Angle);

		FVector ActorPosition = GetActorLocation() + FVector(FMath::Cos(RadianAngle), FMath::Sin(RadianAngle), 0) * Radius + Offset;

		ActorLocation[i] = ActorPosition;

		if (Actors[i]) {
			Actors[i]->SetActorLocation(ActorLocation[i]);
			Actors[i]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ACAS_SwitchLinkedFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwitchOn) {
		return;
	}
	else {
		if (bInverse) {
			AddActorLocalRotation(FRotator(0, -Degree, 0));
		}
		else {
			AddActorLocalRotation(FRotator(0, Degree, 0));
		}
	}
}
