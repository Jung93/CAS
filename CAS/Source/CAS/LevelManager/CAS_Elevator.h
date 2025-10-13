// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_Elevator.generated.h"

UCLASS()
class CAS_API ACAS_Elevator : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAS_Elevator();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartLocation;
	float ElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 300.0f; 

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WaitTime = 2.0f;

	bool MovingUp = false;
	bool Waiting = true;

	FTimerHandle WaitTimerHandle;

	void ToggleDirection();
	void StartMove();
};
