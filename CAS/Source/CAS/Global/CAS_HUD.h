// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CAS_HUD.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAS_TitleWidget> TitleWidgetClass;

protected:
	UPROPERTY()
	class UCAS_TitleWidget* TitleWidget;
};
