// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"	
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "CAS_EnemyController.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Nomal,
	Aggressive,//공격적인 성향
	Scared,//적을 보면 도주하는 성향
	Mild, //
};

UCLASS()
class CAS_API ACAS_EnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAS_EnemyController();

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	float GetSightRange() { return SightConfig->SightRadius; }
	class UCAS_BehaviorComponent* GetBehaviorComponent() { return BehaviorComponent; }
	
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

	void SetMeshColor(APawn * pawn,FVector colorVector , FName name = "Tint");
protected:
	FVector OriginalColorVector = FVector(1,1,1);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;	

	UPROPERTY(EditAnywhere)
	class UCAS_BehaviorComponent* BehaviorComponent;


};
