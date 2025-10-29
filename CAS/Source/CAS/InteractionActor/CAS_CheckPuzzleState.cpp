// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_CheckPuzzleState.h"
#include "CAS_CheckPuzzleState.h"
#include "LevelManager/CAS_LaserTarget.h"
#include "LevelManager/CAS_MiniStageDoor.h"

void ACAS_CheckPuzzleState::InteractionWithPlayer()
{
	if (!Target->IsTargetInCollider()) {
		return;
	}
	bool bClear = Target->CheckPuzzleState();
	if (bClear) {
		Door->DoorOpen();
	}
	Target->SetTargetColor(bClear);
}
