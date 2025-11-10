// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_CheckPuzzleState.h"
#include "CAS_CheckPuzzleState.h"
#include "LevelManager/CAS_LaserTarget.h"
#include "LevelManager/CAS_MiniStageDoor.h"
#include "InteractionActor/CAS_InteractionMirror.h"

void ACAS_CheckPuzzleState::BeginPlay()
{
	Super::BeginPlay();

	auto Material = StaticMesh->GetMaterial(0);

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		StaticMesh->SetMaterial(0, DynamicMaterial);

		FMaterialParameterInfo ParamInfo(TEXT("Color"));
		DynamicMaterial->GetVectorParameterValue(ParamInfo, InitColor);

	}
}

void ACAS_CheckPuzzleState::InteractionWithPlayer()
{
	if (!Target->IsTargetInCollider()) {
		SetInteractionColor(false);
		return;
	}
	bool bClear = CheckPuzzleState();
	if (bClear) {
		Door->DoorOpen();
	}
	Target->SetTargetColor(bClear);
	SetInteractionColor(bClear);
}

bool ACAS_CheckPuzzleState::CheckPuzzleState()
{
	for (auto Mirror : Mirrors) {
		if (!Mirror->GetLaserActivated()) {
			ResetAllMirrors();
			Target->SetTargetInCollider(false);
			return false;
		}
	}

	for (auto Mirror : Mirrors) {
		Mirror->SetLaserActivated(false);
	}
	return true;
}

void ACAS_CheckPuzzleState::SetInteractionColor(bool PuzzleSolved)
{
	if (PuzzleSolved) {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.6f, 0.0f, 1.0f));
	}
	else {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
	}

	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, this, &ThisClass::SetInitColor, 2.0f, false);
}

void ACAS_CheckPuzzleState::SetInitColor()
{
	DynamicMaterial->SetVectorParameterValue(FName("Color"), InitColor);
	
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimer);
}

void ACAS_CheckPuzzleState::ResetAllMirrors()
{
	for (auto Mirror : Mirrors) {
		Mirror->ResetMirrorTransform();
	}
}
