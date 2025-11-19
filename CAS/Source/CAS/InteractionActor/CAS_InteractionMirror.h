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

	void SetLaserActivated(bool LaserActivated);
	bool GetLaserActivated();
	void ResetMirrorTransform();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	float Degree = 5.0f;
protected:
	UPROPERTY(EditAnywhere)
	class UCAS_LaserComponent* LaserComponent;

	FTransform InitTransform;

	UPROPERTY(EditAnywhere,meta = (MakeEditWidget = "true"))
	FVector TargetLocation = FVector::ZeroVector;
};
