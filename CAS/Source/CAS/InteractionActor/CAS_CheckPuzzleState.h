// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "CAS_CheckPuzzleState.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_CheckPuzzleState : public ACAS_InteractionActor
{
	GENERATED_BODY()
	
public:
	virtual void InteractionWithPlayer()override;

private:
	UPROPERTY(EditAnywhere)
	class ACAS_LaserTarget* Target;
	UPROPERTY(EditAnywhere)
	class ACAS_PressureSwitchLinkedDoor* Door;

};
