// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_LoadingOnlyActor.generated.h"

UCLASS()
class CAS_API ACAS_LoadingOnlyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_LoadingOnlyActor();

	UPROPERTY(EditAnywhere)
	float Interval = 0.5f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckReady();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> LoadingUIClass;
	UPROPERTY()
	class UUserWidget* LoadingUI;

	UPROPERTY()
	FTimerHandle Timer;
};
