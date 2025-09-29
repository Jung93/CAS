// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_PortalActor.generated.h"

UCLASS()
class CAS_API ACAS_PortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_PortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UFUNCTION()
	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);

public:	
	UPROPERTY(EditAnywhere, Category = "NextLevel")
	class UWorld* NextLevel;
	
	FName GetNextLevelName() { return NextLevelName; }
private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* PortalCollider;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* PortalEffect;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootScene;
private:
	FName NextLevelName;
};
