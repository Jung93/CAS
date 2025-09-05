// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_PlayMontageTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_PlayMontageTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCAS_PlayMontageTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector IsMontagePlayingKey;
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* CAS_Montage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	float LoopTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Montage")
	bool bPlayOnce = false;

private:
	float CurrentTime = 0.0f;

	UBehaviorTreeComponent* CachedOwnerComp;
};
