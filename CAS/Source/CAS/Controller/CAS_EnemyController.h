// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"	
#include "Perception/AISenseConfig_Sight.h"
#include "CAS_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAS_EnemyController();

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;
	UFUNCTION()
	void RandMove();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardData* BlackboardData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardComponent* BlackBoardComponent;
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);
protected:
	UPROPERTY()
	FTimerHandle TimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

};
