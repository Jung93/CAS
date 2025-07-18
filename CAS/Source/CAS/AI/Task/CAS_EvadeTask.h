// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_EvadeTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_EvadeTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_EvadeTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key;
};
