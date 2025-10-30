// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "CAS_LaserComponent.generated.h"


UCLASS( ClassGroup=(Laser), meta=(BlueprintSpawnableComponent) )
class CAS_API UCAS_LaserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCAS_LaserComponent();

	void SetLaserActivated(bool bActivate) { bLaserActivated = bActivate; }
	bool GetLaserActivated() const { return bLaserActivated; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void TraceLaser(float DeltaTime);
	void ClearChildActor();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool HasLaserHitChildActor() { return ChildActor != nullptr; }
	AActor* GetDetectedActor();
protected:
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere)
	float Offset = 2000.0f;

	UPROPERTY()
	AActor* ChildActor = nullptr;

	UPROPERTY(EditAnywhere)
	bool bLaserActivated = false;

	FVector LaserEnd;		
};
