// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_SpawnerActor.generated.h"

UCLASS()
class CAS_API ACAS_SpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_SpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestorySpawnerActor();
protected:
	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, Category = "ActorPosition", meta = (MakeEditWidget = "true"))
	FVector RelativePosition = FVector::ZeroVector;

	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	int32 SpawnCount = 0;

	int32 RemainingSpawnNum = 0;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	float Interval = 0.0f;

protected:
	UFUNCTION()
	void OnDestroyedEvent(AActor* Actor);

	void SpawnNewActor();
	
	UFUNCTION()
	void InitialSpawn();
};
