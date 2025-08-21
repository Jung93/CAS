// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_LevelConversion.generated.h"

UCLASS()
class CAS_API ACAS_LevelConversion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_LevelConversion();

	UFUNCTION()
	void LoadLevel(FName levelName);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Loader")
	FName NextLevelName;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Loader")
	class UBoxComponent* volume;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
