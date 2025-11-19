// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Controller/CAS_PlayerController.h"
#include "CAS_InteractionActor.generated.h"



UCLASS(Abstract)
class CAS_API ACAS_InteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_InteractionActor();

	bool CanInteraction() { return bCanInteraction; }
	virtual void InteractionWithPlayer() PURE_VIRTUAL(ACAS_InteractionActor::InteractionWithPlayer, ;); 
	class UStaticMeshComponent* GetMesh() { return StaticMesh; };
	void ChangeTexture(EInputDeviceType InputDevice);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Collider")
	class USphereComponent* SenseCollider;



protected:
	//상호작용
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCAS_KeyPressUI> KeyPressUIClass;
	UPROPERTY()
	class UCAS_KeyPressUI* KeyPressUI = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* KeyPressWidgetComponent;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UTexture2D* KeyTexture;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UTexture2D* ControllerKeyTexture;


	bool bCanInteraction = false;

protected:
	//mesh
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

};
