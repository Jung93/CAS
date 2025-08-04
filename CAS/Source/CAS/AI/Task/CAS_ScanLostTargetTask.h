// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_ScanLostTargetTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_ScanLostTargetTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_ScanLostTargetTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector MovePositionKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

protected:
	UPROPERTY(EditAnywhere)
	float PlayTime = 5.0f;
	float CurrTime = 0.0f;
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

};