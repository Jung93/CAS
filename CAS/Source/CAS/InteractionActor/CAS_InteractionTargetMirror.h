// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionMirror.h"
#include "CAS_InteractionTargetMirror.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_InteractionTargetMirror : public ACAS_InteractionMirror
{
	GENERATED_BODY()
public:
	ACAS_InteractionTargetMirror();

protected:
	virtual void BeginPlay() override;
	virtual void WidgetClicked(EWidgetPositionType Type) override;

	void RotationToDir(FVector Location);
protected:
	UPROPERTY(EditAnywhere)
	class ACAS_SwitchLinkedFloor* TargetFloor;

	UPROPERTY()
	TArray<FVector> TargetActorLocation;

	int32 TargetIndex = 0;
	int32 ArrayNum = 0;
};
