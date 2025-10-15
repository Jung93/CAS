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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SwitchPressed(bool SwithOn);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Switch")
	ACAS_PressureSwitch* SwitchInstance = nullptr;
};
