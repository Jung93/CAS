// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"	
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
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
	virtual void Tick(float DeltaSeconds) override;

	float GetSightRange() { return SightConfig->SightRadius; }
	class UCAS_BehaviorComponent* GetBehaviorComponent() { return BehaviorComponent; }
	
	bool bUseDebug = true;
	bool bDebugOn = false;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardData* BlackboardData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardComponent* BlackBoardComponent;
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);
	void SetMeshColor(APawn * pawn,FVector colorVector , FName name = "Tint");
protected:
	FVector OriginalColorVector = FVector(1,1,1);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(EditAnywhere)
	class UCAS_BehaviorComponent* BehaviorComponent;


};
