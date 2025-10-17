// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedGate.h"
#include "InteractionActor/CAS_InteractionSwitch.h"
#include "Character/CAS_Hat.h"

ACAS_SwitchLinkedGate::ACAS_SwitchLinkedGate()
{
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StaticMesh;

}

void ACAS_SwitchLinkedGate::BeginPlay()
{
	Super::BeginPlay();

	if (SwitchInstance2) {
		SwitchInstance2->OnSwitchClicked.AddUObject(this, &ThisClass::SwitchClicked);
	}

	StartPosition = GetActorLocation();
	EndPosition = StartPosition + FVector(0.f, 0.f, MoveDistance);

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

}

void ACAS_SwitchLinkedGate::SwitchClicked(bool SwitchOn)
{
	if (IsOpen == SwitchOn)
		IsAnotherSwitch = true;

	IsOpen = IsAnotherSwitch ? !SwitchOn : SwitchOn;


	IsWaiting = false;
}

void ACAS_SwitchLinkedGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsWaiting)
		return;

	FVector CurrentLocation = GetActorLocation();

	float Direction = IsOpen ? 1.f : 0.f;
	FVector TargetLocation = StartPosition + FVector(0.f, 0.f, Direction * MoveDistance);

	// 한 프레임 이동
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(NewLocation);


	if (FVector::Dist(NewLocation, EndPosition) < 1.f)
	{
		IsWaiting = true;
		IsAnotherSwitch = false;
	}

}

void ACAS_SwitchLinkedGate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{

	auto hat = Cast<ACAS_Hat>(OtherActor);

	if (!hat->IsValidLowLevel())
		return;

	//if (!hat->GetIsThrowing())
	//	return;

	hat->SetReturning();
	hat->Return();


}

