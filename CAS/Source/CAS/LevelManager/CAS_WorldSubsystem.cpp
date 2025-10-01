// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_WorldSubsystem.h"

void UCAS_WorldSubsystem::CheckGameProgress()
{
	if (CompletedTargetCount == SpawnedTargetCount) {
		OnPuzzleCompleted.Broadcast();
	}
	auto temp = 1;
}
