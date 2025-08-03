// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_SetNextMovePoint.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SetNextMovePoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_SetNextMovePoint();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector MovePositionKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;
	
};
