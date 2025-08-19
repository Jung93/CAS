// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_ActiveAbilityTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_ActiveAbilityTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCAS_ActiveAbilityTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key;
};
