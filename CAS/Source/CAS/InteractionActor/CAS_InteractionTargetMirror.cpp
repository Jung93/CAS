// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionTargetMirror.h"
#include "LevelManager/CAS_SwitchLinkedFloor.h"
#include "Kismet/KismetMathLibrary.h"

ACAS_InteractionTargetMirror::ACAS_InteractionTargetMirror()
{
}

void ACAS_InteractionTargetMirror::BeginPlay()
{
	//위치정보만 받아와서저장

	Super::BeginPlay();

	if (TargetFloor) {
		const auto& TargetActors = TargetFloor->GetOwningActors();

		for (auto TargetActor : TargetActors) {
			FVector ActorLocation = TargetActor->GetActorLocation();
			if (ActorLocation == GetActorLocation()) {
				continue;
			}
			TargetActorLocation.Add(ActorLocation);
		}
		ArrayNum = TargetActors.Num();

		RotationToDir(TargetActorLocation[TargetIndex]);
	}

}

void ACAS_InteractionTargetMirror::WidgetClicked(EWidgetPositionType Type)
{
	//좌우 레이저 타겟 정해서 쏘기

	if (Type == EWidgetPositionType::NONE) {
		return;
	}
	else if (Type == EWidgetPositionType::Left) {
		TargetIndex--;

		if (TargetIndex < 0) {
			TargetIndex += ArrayNum;
		}
	}
	else if (Type == EWidgetPositionType::Right) {
		TargetIndex++;
		
		TargetIndex %= ArrayNum;
	}
	
	RotationToDir(TargetActorLocation[TargetIndex]);
}

void ACAS_InteractionTargetMirror::RotationToDir(FVector Location)
{
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);

	SetActorRotation(LookAtRot);
}
