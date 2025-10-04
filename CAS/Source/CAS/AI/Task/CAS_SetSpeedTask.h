// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Character/CAS_Character.h"
#include "CAS_SetSpeedTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SetSpeedTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_SetSpeedTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveType MoveType;
	
};
