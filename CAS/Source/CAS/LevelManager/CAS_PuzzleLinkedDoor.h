// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_PuzzleLinkedDoor.generated.h"

UCLASS()
class CAS_API ACAS_PuzzleLinkedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAS_PuzzleLinkedDoor();

protected:
	virtual void BeginPlay() override;

	void DoorOpen();


private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

};
