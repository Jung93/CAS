// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_SwitchLinkedActor.generated.h"

UCLASS(Abstract)
class CAS_API ACAS_SwitchLinkedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_SwitchLinkedActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SwitchClicked(bool SwitchOn) PURE_VIRTUAL(ACAS_SwitchLinkedActor::SwitchClicked, );
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Switch")
	TSubclassOf<class ACAS_InteractionSwitch> SwitchClass;
	
	UPROPERTY()
	class ACAS_InteractionSwitch* SwitchInstance = nullptr;

};
