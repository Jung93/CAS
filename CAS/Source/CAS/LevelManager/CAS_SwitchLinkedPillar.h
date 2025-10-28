// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_SwitchLinkedActor.h"
#include "CAS_SwitchLinkedPillar.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_SwitchLinkedPillar : public ACAS_SwitchLinkedActor
{
	GENERATED_BODY()
	
public:
	ACAS_SwitchLinkedPillar();
protected:
	void BeginPlay() override;

	virtual void SwitchClicked(bool SwitchOn) override;

	void SetTargetLocation();
protected:
	UPROPERTY(EditAnywhere)
	float Offset = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	FVector StartPosition;
	FVector EndPosition;

protected:
	FTimerHandle MoveHandle;

	UPROPERTY(EditAnywhere)
	float MaxDuration = 0.3f;
	UPROPERTY(EditAnywhere)
	float Interval = 0.02f;

	float CurrDuration = 0.0f;
};
