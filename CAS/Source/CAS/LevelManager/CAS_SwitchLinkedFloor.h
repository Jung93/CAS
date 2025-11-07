// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_SwitchLinkedActor.h"
#include "CAS_SwitchLinkedFloor.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_SwitchLinkedFloor : public ACAS_SwitchLinkedActor
{
	GENERATED_BODY()
public:
	ACAS_SwitchLinkedFloor();
protected:
	virtual void SwitchClicked(bool SwitchOn) override;

	virtual void BeginPlay()override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> Actors;

	TArray<FVector> ActorLocation;

	bool bSwitchOn = false;
protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere)
	float Radius = 0.0;
	UPROPERTY(EditAnywhere)
	float Degree = 1.0f;
	UPROPERTY(EditAnywhere , meta = (ClampMin = 0.1f, ClampMax = 1.0f))
	float Param = 0.7f;
	UPROPERTY(EditAnywhere)
	bool bInverse = false;
	UPROPERTY(EditAnywhere)
	FVector Offset = FVector(0, 0, 100.0f);
};
