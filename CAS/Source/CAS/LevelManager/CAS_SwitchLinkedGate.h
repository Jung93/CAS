// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_SwitchLinkedActor.h"
#include "CAS_SwitchLinkedGate.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_SwitchLinkedGate : public ACAS_SwitchLinkedActor
{
	GENERATED_BODY()
	
public:
	ACAS_SwitchLinkedGate();

protected:
	void BeginPlay() override;

	virtual void SwitchClicked(bool SwitchOn) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);


private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Switch")
	class ACAS_InteractionSwitch* SwitchInstance2 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance = 200.0f;

	FVector StartPosition;
	FVector EndPosition;

	bool IsOpen = false;
	bool IsWaiting = true;
	bool IsAnotherSwitch = false;
};
