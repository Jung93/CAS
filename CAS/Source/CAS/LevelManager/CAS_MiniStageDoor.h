// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_StageDoor.h"
#include "CAS_MiniStageDoor.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_MiniStageDoor : public ACAS_StageDoor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

};
