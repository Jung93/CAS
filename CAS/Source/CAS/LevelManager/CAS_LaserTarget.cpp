// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserTarget.h"
#include "LevelManager/CAS_WorldSubsystem.h"

// Sets default values
ACAS_LaserTarget::ACAS_LaserTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void ACAS_LaserTarget::BeginPlay()
{
	Super::BeginPlay();
	
	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->RegisterTarget();

	auto Material = StaticMesh->GetMaterial(0);

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

		StaticMesh->SetMaterial(0, DynamicMaterial);

		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
	}
}

void ACAS_LaserTarget::LaserReached()
{
	if (bTargetInCollider) {
		return;
	}
	DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.6f, 0.0f, 1.0f));

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->PlusCompletedCount();

	bTargetInCollider = true;
}

