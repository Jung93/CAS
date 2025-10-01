// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SpawnerActor.h"

// Sets default values
ACAS_SpawnerActor::ACAS_SpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ACAS_SpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (SpawnActorClass) {
		SpawnTransform = GetTransform();
		FVector	SpawnPosition = GetActorTransform().TransformPosition(RelativePosition);
		SpawnTransform.SetLocation(SpawnPosition);
		
		RemainingSpawnedNum = SpawnCount;

		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::InitialSpawn, Interval, true);
	}

}

void ACAS_SpawnerActor::OnDestroyedEvent(AActor* Actor)
{
	SpawnNewActor();
}

void ACAS_SpawnerActor::SpawnNewActor()
{
	auto SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, SpawnTransform);
	SpawnedActor->OnDestroyed.AddDynamic(this, &ThisClass::OnDestroyedEvent);
}

void ACAS_SpawnerActor::InitialSpawn()
{
	if (RemainingSpawnedNum <= 0) {
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
	else {
		RemainingSpawnedNum--;
		SpawnNewActor();
	}
}
