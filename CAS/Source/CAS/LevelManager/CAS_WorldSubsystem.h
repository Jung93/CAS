// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CAS_WorldSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStageCompleted);
DECLARE_MULTICAST_DELEGATE(FWithdrawStageCompleted);
//auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
UCLASS()
class CAS_API UCAS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterTarget() { SpawnedTargetCount++; }
	void PlusCompletedCount(bool bPuzzle) { CompletedTargetCount++; CheckGameProgress(bPuzzle); }
	void SubCompletedCount(bool bPuzzle) { CompletedTargetCount--;  CheckGameProgress(bPuzzle); } 
	
	void StageClearEvent();
	void WithdrawClearEvent();

	FOnStageCompleted OnStageCompleted;
	FWithdrawStageCompleted WithdrawStageCompleted;
private:
	UFUNCTION()
	void CheckGameProgress(bool bPuzzle);

	int32 SpawnedTargetCount = 0;
	int32 CompletedTargetCount = 0;
};