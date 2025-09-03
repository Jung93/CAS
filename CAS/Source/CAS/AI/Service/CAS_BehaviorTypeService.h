// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CAS_BehaviorTypeService.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_BehaviorTypeService : public UBTService
{
	GENERATED_BODY()
public:
	UCAS_BehaviorTypeService();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector PlayerKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector IsMontagePlayingKey;
};
