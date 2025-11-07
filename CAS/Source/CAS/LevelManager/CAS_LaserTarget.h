// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_LaserTarget.generated.h"

UCLASS()
class CAS_API ACAS_LaserTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_LaserTarget();
	bool IsTargetInCollider() { return bTargetInCollider; }
	void SetTargetInCollider(bool TargetInCollider) { bTargetInCollider = TargetInCollider; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	void LaserReached();
	void LaserUnreachable();
	void SetTargetColor(bool Reached);
private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	bool bTargetInCollider = false;	
};
