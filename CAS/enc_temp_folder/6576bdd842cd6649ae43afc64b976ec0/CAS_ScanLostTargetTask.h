// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_ScanLostTargetTask.generated.h"

/**
 * 
 */
USTRUCT()
struct FScanTimeMemory
{
	GENERATED_BODY()

public:
	float CurTime = 0.0f;
};

UCLASS()
class CAS_API UCAS_ScanLostTargetTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_ScanLostTargetTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FScanTimeMemory); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector BehaviorTypeKey;

protected:
	UPROPERTY(EditAnywhere)
	float PlayTime = 5.0f;
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

};