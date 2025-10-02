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
//적 3명 각각 비긴플레이에서 resister 시킨다음 -> 3
//적 각각 비긴플레이에서 사망시 델리게이트로 -> 사망카운트 ++ + 카운트 올리면서 검사