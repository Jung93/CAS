// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedRotationPillar.h"

void ACAS_SwitchLinkedRotationPillar::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = GetActorRotation();
}

void ACAS_SwitchLinkedRotationPillar::SwitchClicked(bool SwitchOn)
{
	if (SwitchOn) {
		EndRotation = StartRotation + FRotator(0, 180.0f, 0);
	}
	else {
		EndRotation = StartRotation;
	}
	GetWorldTimerManager().SetTimer(MoveHandle, this, &ThisClass::SetTargetRotation, Interval, true);

}

void ACAS_SwitchLinkedRotationPillar::SetTargetRotation()
{
	CurrDuration += Interval;

	float Alpha = FMath::Clamp(CurrDuration / MaxDuration, 0.0f, 1.0f);

	FRotator TargetRotation = FMath::Lerp(GetActorRotation(), EndRotation, Alpha);

	SetActorRotation(TargetRotation);

	if (Alpha >= 1.0f) {
		CurrDuration = 0.0f;
		GetWorldTimerManager().ClearTimer(MoveHandle);
	}

}