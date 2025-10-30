// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_WorldSubsystem.h"
#include "Global/CAS_GameInstance.h"

void UCAS_WorldSubsystem::StageClearEvent()
{
	OnPuzzleCompleted.Broadcast();

	UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(GetWorld()->GetGameInstance());

	gi->ClearCurrentPuzzle();
}

void UCAS_WorldSubsystem::CheckGameProgress()
{
	if (CompletedTargetCount == SpawnedTargetCount) {
		StageClearEvent();
	}
}
