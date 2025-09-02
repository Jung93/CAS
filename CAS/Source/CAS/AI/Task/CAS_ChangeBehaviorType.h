// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/CAS_BehaviorComponent.h"
#include "CAS_ChangeBehaviorType.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_ChangeBehaviorType : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector BehaviorTypeKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBehaviorType BehaviorType;
};
