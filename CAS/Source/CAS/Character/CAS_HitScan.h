// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_HitScan.generated.h"

UCLASS()
class CAS_API ACAS_HitScan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_HitScan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwnerClass(class ACAS_Character* owner) { OwnerClass = owner; };
	class ACAS_Character* GetOwnerClass() { return OwnerClass; }
	class USphereComponent* GetCollision() { return Collision; }


private:
	UPROPERTY()
	class ACAS_Character* OwnerClass = nullptr;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;

	UFUNCTION()
	void NotifyCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
