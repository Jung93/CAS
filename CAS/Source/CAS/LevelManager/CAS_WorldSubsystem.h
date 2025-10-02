// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CAS_WorldSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPuzzleCompleted);
//auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
UCLASS()
class CAS_API UCAS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterTarget() { SpawnedTargetCount++; }
	void PlusCompletedCount() { CompletedTargetCount++; CheckGameProgress(); }
	void SubCompletedCount() { CompletedTargetCount--; }
	void StageClearEvent();
	FOnPuzzleCompleted OnPuzzleCompleted;
private:
	UFUNCTION()
	void CheckGameProgress();

	int32 SpawnedTargetCount = 0;
	int32 CompletedTargetCount = 0;


};
