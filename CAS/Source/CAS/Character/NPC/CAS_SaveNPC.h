// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "CAS_SaveNPC.generated.h"

UCLASS()
class CAS_API ACAS_SaveNPC : public ACAS_InteractionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_SaveNPC();

	virtual void InteractionWithPlayer() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult) override;
	virtual void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCAS_SaveLoadWidget> SaveLoadWidgetClass;
	UPROPERTY()
	class UCAS_SaveLoadWidget* SaveLoadWidget;
};
