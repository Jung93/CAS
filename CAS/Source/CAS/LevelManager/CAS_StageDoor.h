// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_StageDoor.generated.h"

UCLASS()
class CAS_API ACAS_StageDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_StageDoor();

	void DoorOpen();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DoorOpenEvent(float DeltaTime);
	void DoorCloseEvent(float DeltaTime);

	FVector DoorPosition;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	
	UPROPERTY(EditAnywhere, Category = "Door")
	UStaticMeshComponent* Base__Mesh;

	UPROPERTY(EditAnywhere, Category = "Door")
	UStaticMeshComponent* Door__Mesh;

	UPROPERTY(EditAnywhere, Category = "Door")
	float Offset = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float MoveSpeed = 5.0f;

	bool bDoorOpen = false;
};
