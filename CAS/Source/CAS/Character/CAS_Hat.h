// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CAS_Hat.generated.h"

UCLASS()
class CAS_API ACAS_Hat : public APawn
{
	GENERATED_BODY()

public:
	ACAS_Hat();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Ready();

	UFUNCTION()
	void OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

	UFUNCTION()
	void Throw(const FVector& direction);

	UFUNCTION()
	void ThrowAndReturn(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Return();

	UFUNCTION(BlueprintCallable)
	void SetReturning() { _isReturning = true; }

	UFUNCTION()
	void SetPlayer(class ACAS_Player* player) { _player = player; }

	UFUNCTION()
	class ACAS_Player* GetPlayer() { return _player; }

	bool GetIsReady() { return IsReady; }

	UFUNCTION(BlueprintCallable)
	bool GetIsThrowing() { return _isThrowing; }
	bool GetIsReturning() { return _isReturning; }

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USphereComponent* _collider;

	UPROPERTY(VisibleAnywhere, Category = "TestCaptureTarget")
	class ACAS_EnemyCapt* _testCaptureTarget;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class ACAS_Player* _player;

	FVector StartLocation;
	FVector TargetLocation;
	FVector MoveDirection;

	bool IsReady = false;
	bool _isThrowing = false;
	bool _isReturning = false;
	float _capturingTime = 0.0f;
	float _totalMoveTime = 1.0f;
};
