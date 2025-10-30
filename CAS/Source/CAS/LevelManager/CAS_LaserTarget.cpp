// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserTarget.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionActor/CAS_InteractionMirror.h"

// Sets default values
ACAS_LaserTarget::ACAS_LaserTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(StaticMesh);
}

bool ACAS_LaserTarget::CheckPuzzleState()
{
	for (auto Mirror : Mirrors) {
		if (!Mirror->GetLaserActivated()) {
			ResetAllMirrors();
			bTargetInCollider = false;			
			return false;
		}
	}

	for (auto Mirror : Mirrors) {
		Mirror->SetLaserActivated(false);
	}
	return true;
}

// Called when the game starts or when spawned
void ACAS_LaserTarget::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MirrorClass, Actors);

	for (auto Actor : Actors)
	{
		if (auto Target = Cast<ACAS_InteractionMirror>(Actor))
		{
			Mirrors.Add(Target);
		}
	}

	auto Material = StaticMesh->GetMaterial(0);

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

		StaticMesh->SetMaterial(0, DynamicMaterial);

		SetTargetColor(false);
	}
}

void ACAS_LaserTarget::ResetAllMirrors()
{
	for (auto Mirror : Mirrors) {
		Mirror->ResetMirrorTransform();
	}
}

void ACAS_LaserTarget::LaserReached()
{
	if (bTargetInCollider) {
		return;
	}
	SetTargetColor(true);

	bTargetInCollider = true;

}

void ACAS_LaserTarget::LaserUnreachable()
{
	SetTargetColor(false);

	bTargetInCollider = false;
}

void ACAS_LaserTarget::SetTargetColor(bool Reached)
{
	if (Reached) {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.6f, 0.0f, 1.0f));
	}
	else {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
	}
}

