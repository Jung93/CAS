// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_Elevator.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Character/CAS_Player.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Global/CAS_GameInstance.h"

ACAS_Elevator::ACAS_Elevator()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	InvisibleDoor = CreateDefaultSubobject<UBoxComponent>("Door");

	RootComponent = StaticMesh;
	Collider->SetupAttachment(StaticMesh);
	InvisibleDoor->SetupAttachment(StaticMesh);
}

void ACAS_Elevator::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnPuzzleCompleted.AddUObject(this, &ThisClass::Active);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InvisibleDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto gi = Cast<UCAS_GameInstance>(GetGameInstance());

	if (gi && gi->IsPuzzleClear())
	{
		Active();
	}

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
		InvisibleDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

void ACAS_Elevator::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<ACAS_Player>(OtherActor);

	if (player)
	{
		StartMove();
		InvisibleDoor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

}

void ACAS_Elevator::StartMove()
{
	Waiting = false;
	MovingUp = !MovingUp;
}

void ACAS_Elevator::Active()
{
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

