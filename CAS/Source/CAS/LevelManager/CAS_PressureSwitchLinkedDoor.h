// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager/CAS_PressureSwitch.h"
#include "CAS_PressureSwitchLinkedDoor.generated.h"

UCLASS()
class CAS_API ACAS_PressureSwitchLinkedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_PressureSwitchLinkedDoor();

	void SwitchPressed(bool SwitchOn);
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
	ACAS_PressureSwitch* SwitchInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "Door")
	UStaticMeshComponent* Base_Mesh;

	UPROPERTY(EditAnywhere, Category = "Door")
	UStaticMeshComponent* Door_Mesh;

	UPROPERTY(EditAnywhere, Category = "Door")
	float Offset = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float MoveSpeed = 10.0f;

	bool bDoorOpen = false;
};
