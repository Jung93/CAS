// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "CAS_InteractionMirror.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_InteractionMirror : public ACAS_InteractionActor
{
	GENERATED_BODY()
public:
	ACAS_InteractionMirror();
	virtual void InteractionWithPlayer()override;

	FVector GetMirrorForwardVector() { return GetActorForwardVector(); }

	bool GetLaserActivated() { return bLaserActivated; }
	void SetLaserActivated(bool LaserActivated) { bLaserActivated = LaserActivated; }
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditAnywhere)
	float Degree = 5.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;

private:
	FVector LaserEnd;

	float Offset = 2000.0f;

	bool bLaserActivated = false;

	ACAS_InteractionMirror* ChildLaser = nullptr;
};
