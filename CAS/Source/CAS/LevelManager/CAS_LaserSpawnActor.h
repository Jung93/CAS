// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_LaserSpawnActor.generated.h"

UCLASS()
class CAS_API ACAS_LaserSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAS_LaserSpawnActor();

	void SetLaserActivated(bool LaserActivated);
	bool GetLaserActivated();
protected:
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UCAS_LaserComponent* LaserComponent;
};
