// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_BallTargetPoint.generated.h"

UCLASS()
class CAS_API ACAS_BallTargetPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_BallTargetPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UFUNCTION()
	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BallDetectCollider;
private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
