// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_PatrolPath.generated.h"

UCLASS()
class CAS_API ACAS_PatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAS_PatrolPath();

	FVector GetPatrolPoint(int32 index) { return PatrolPath[index]; }
	int32 GetPatrolLength() { return PatrolPath.Num(); }
	int32 GetPathIndex() { return PathIndex; }

	void IncreasePathIndex() { PathIndex = (PathIndex + 1) % GetPatrolLength(); }
	void DecreasePathIndex(){ PathIndex = (PathIndex - 1 + GetPatrolLength()) % GetPatrolLength(); }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "PatrolPath", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolPath;

	UPROPERTY()
	int32 PathIndex = 0;	
};
