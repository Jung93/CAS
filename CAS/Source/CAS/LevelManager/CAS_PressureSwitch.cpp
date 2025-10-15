// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_PressureSwitch.h"
#include "Components/BoxComponent.h"
#include "Character/CAS_Character.h"
// Sets default values
ACAS_PressureSwitch::ACAS_PressureSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));

	SetRootComponent(BaseMesh);
	SwitchMesh->SetupAttachment(BaseMesh);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(SwitchMesh);
	TriggerVolume->SetGenerateOverlapEvents(true);
}

void ACAS_PressureSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwitchPressed) {
		SwitchPressedEvent(DeltaTime);
	}
	else {
		SwitchReleasedEvent(DeltaTime);
	}
}

// Called when the game starts or when spawned
void ACAS_PressureSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	SwitchPosition = SwitchMesh->GetRelativeLocation();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACAS_PressureSwitch::OnOverlapEvent);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ACAS_PressureSwitch::EndOverlapEvent);
}

void ACAS_PressureSwitch::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) {
		return;
	}
	if (auto Character = Cast<ACAS_Character>(OtherActor)) {
		TotalMass += 80.0f;
	}
	else {
		TArray<UPrimitiveComponent*> Components;
		OtherActor->GetComponents<UPrimitiveComponent>(Components);

		for (UPrimitiveComponent* Comp : Components)
		{
			if (Comp->IsSimulatingPhysics())
			{
				TotalMass += Comp->GetMass();
			}
		}

	}
	if (TotalMass >= TargetMass) {
		bSwitchPressed = true;
		OnSwitchPressed.Broadcast(bSwitchPressed);
	}
}

void ACAS_PressureSwitch::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) {
		return;
	}
	if (auto Character = Cast<ACAS_Character>(OtherActor)) {
		TotalMass -= 80.0f;
	}
	else {
		TArray<UPrimitiveComponent*> Components;
		OtherActor->GetComponents<UPrimitiveComponent>(Components);

		for (UPrimitiveComponent* Comp : Components)
		{
			if (Comp->IsSimulatingPhysics())
			{
				TotalMass -= Comp->GetMass();
			}
		}

	}

	if (TotalMass < TargetMass) {
		bSwitchPressed = false;
		OnSwitchPressed.Broadcast(bSwitchPressed);
	}
}

void ACAS_PressureSwitch::SwitchPressedEvent(float DeltaTime)
{
	FVector TargetLocation = SwitchPosition;

	TargetLocation.Z -= Offset;

	FVector CurrentLocation = SwitchMesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	SwitchMesh->SetRelativeLocation(NextLocation);
}

void ACAS_PressureSwitch::SwitchReleasedEvent(float DeltaTime)
{
	FVector TargetLocation = SwitchPosition;

	FVector CurrentLocation = SwitchMesh->GetRelativeLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	SwitchMesh->SetRelativeLocation(NextLocation);
}

