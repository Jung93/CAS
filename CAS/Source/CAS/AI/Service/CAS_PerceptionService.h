// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CAS_PerceptionService.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_PerceptionService : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	FTimerHandle TrackingTimerHandle;

	float TrackingTime = 5.0f;

};
