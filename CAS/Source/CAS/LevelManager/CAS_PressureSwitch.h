// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_PressureSwitch.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwitchPressed, bool);

UCLASS()
class CAS_API ACAS_PressureSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_PressureSwitch();

	virtual void Tick(float DeltaTime) override;

	FOnSwitchPressed OnSwitchPressed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SwitchMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Switch")
	float TargetMass = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Switch")
	float Offset = 5.0f;	

	UPROPERTY(EditAnywhere, Category = "Switch")
	float MoveSpeed = 5.0f;		

	FVector SwitchPosition;

	bool bSwitchPressed = false;

	float TotalMass = 0.0f;
protected:
	void SwitchPressedEvent(float DeltaTime);

	void SwitchReleasedEvent(float DeltaTime);
};
