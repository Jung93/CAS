// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAS_SaveNPC.generated.h"

UCLASS()
class CAS_API ACAS_SaveNPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAS_SaveNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanInteraction() { return bCanInteraction; }
protected:
	UFUNCTION()
	void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* NPC_Mesh;

	UPROPERTY(EditAnywhere, Category = "Collider")
	class USphereComponent* SenseCollider;
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCAS_KeyPressUI> KeyPressUIClass;
	UPROPERTY(VisibleAnywhere,Category = "UI")
	class UWidgetComponent* KeyPressWidgetComponent;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UTexture2D* KeyTexture;

	bool bCanInteraction = false;
};
/*
세이브 npc와 대화하면 
1.ui가나오고
2.ui에서 저장하기 누르면 saveloadWidget나옴
3.저장하면 슬롯에 세이브파일 생성 
*/