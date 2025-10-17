// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_SwitchLinkedActor.h"
#include "CAS_SwitchLinkedWheel.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_SwitchLinkedWheel : public ACAS_SwitchLinkedActor
{
	GENERATED_BODY()
	
public:
	ACAS_SwitchLinkedWheel();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void RotateWheel();
protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* WheelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float Degree = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationInterval = 1.0f;

	FTimerHandle RotationTimerHandle;

};
