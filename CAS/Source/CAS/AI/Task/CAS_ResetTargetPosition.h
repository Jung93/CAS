// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_ResetTargetPosition.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_ResetTargetPosition : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_ResetTargetPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector IndexKey;
};
