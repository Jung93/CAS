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
	void LostPlayerInfo();
protected:
	FTimerHandle TrackingTimerHandle;

	UPROPERTY(EditAnywhere)
	float TrackingTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector MovePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector PlayerLostKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector PlayerDetectedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

	class UBlackboardComponent* BlackBoard;

};
