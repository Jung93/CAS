// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_DetectTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_DetectTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(EditAnywhere)
	UAnimMontage* DetectMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector IsMontagePlayingKey;
private:
	UBehaviorTreeComponent* CachedOwnerComp;
};
