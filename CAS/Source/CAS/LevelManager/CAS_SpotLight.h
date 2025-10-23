// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_SpotLight.generated.h"

UCLASS()
class CAS_API ACAS_SpotLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_SpotLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LightOn();

public:	



private:
	UPROPERTY(EditAnywhere)
	class USpotLightComponent* SpotLight;


};
