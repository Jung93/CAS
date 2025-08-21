// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UI/CAS_SkillSlot.h"
#include "CAS_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	float PlayerHP = 0;
	UPROPERTY(VisibleAnywhere)
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere)
	TArray<FCAS_SlotData> QuickSlotData;
	UPROPERTY(VisibleAnywhere)
	FName Level;
};
									