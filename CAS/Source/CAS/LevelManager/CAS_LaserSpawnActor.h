// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "CAS_LaserSpawnActor.generated.h"

UCLASS()
class CAS_API ACAS_LaserSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_LaserSpawnActor();

	void SetLaserDirection(FVector Dir) { InitDir = Dir; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;

	FVector LaserEnd;

	float Offset = 2000.0f;

	FVector	InitDir = FVector::ZeroVector;
};
