// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_WorldSubsystem.h"

void UCAS_WorldSubsystem::StageClearEvent()
{
	OnStageCompleted.Broadcast();
}

void UCAS_WorldSubsystem::WithdrawClearEvent()
{
}

void UCAS_WorldSubsystem::CheckGameProgress(bool bPuzzle)
{
	if (CompletedTargetCount == SpawnedTargetCount) {
		StageClearEvent();
	}
	else {
		WithdrawClearEvent();
	}
}