// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_DestroyVolumeActor.generated.h"

UCLASS()
class CAS_API ACAS_DestroyVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_DestroyVolumeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void DestroyThisActor(AActor* Actor);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	class UBoxComponent* DestroyVolume;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
