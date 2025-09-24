// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionBall.h"

ACAS_InteractionBall::ACAS_InteractionBall()
{
}

void ACAS_InteractionBall::InteractionWithPlayer()
{
}

void ACAS_InteractionBall::BeginPlay()
{
	Super::BeginPlay();
}

void ACAS_InteractionBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAS_InteractionBall::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACAS_InteractionBall::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
