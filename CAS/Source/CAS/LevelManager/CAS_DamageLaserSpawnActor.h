// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager/CAS_LaserSpawnActor.h"
#include "CAS_DamageLaserSpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_DamageLaserSpawnActor : public ACAS_LaserSpawnActor
{
	GENERATED_BODY()
	
public:
	ACAS_DamageLaserSpawnActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DirOffset = 3000.f; // 레이저 길이 속도 등

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageInterval = 1.0f; // 지속 데미지 간격
private:
	UPROPERTY()
	class ACAS_Player* CurrentHitPlayer = nullptr;

	FTimerHandle ContinuousDamageTimerHandle;

	void ApplyContinuousDamage();
	void ClearContinuousDamageTimer();

	FVector ThisLaserEnd;


	FVector	ThisInitDir = FVector::ZeroVector;

};
