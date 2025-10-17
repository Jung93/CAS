// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_Elevator.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "TimerManager.h"

ACAS_Elevator::ACAS_Elevator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACAS_Elevator::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnStageCompleted.AddUObject(this, &ThisClass::StartMove);

}

void ACAS_Elevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Waiting)
		return;

	FVector CurrentLocation = GetActorLocation();

	// 목표 위치 계산
	float Direction = MovingUp ? 1.f : 0.f;
	FVector TargetLocation = StartLocation + FVector(0.f, 0.f, Direction * MoveDistance);

	// 한 프레임 이동
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(NewLocation);

	// 목표 도달 시
	if (FVector::Dist(NewLocation, TargetLocation) < 1.f)
	{
		Waiting = true;
		GetWorldTimerManager().SetTimer(WaitTimerHandle, this, &ACAS_Elevator::ToggleDirection, WaitTime, false);
	}
}

void ACAS_Elevator::ToggleDirection()
{
	MovingUp = !MovingUp;
	Waiting = false;
}

void ACAS_Elevator::StartMove()
{
	Waiting = false;
	MovingUp = true;
}

