// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_EnemyCapt_Kick.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt_Kick : public ACAS_EnemyCapt
{
	GENERATED_BODY()
public:

	ACAS_EnemyCapt_Kick();

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//void ActivateAbility(const FInputActionValue& Value);
	virtual void ActivateEnemyAbility() override;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, Category = "HitScan")
	TSubclassOf<class ACAS_HitScan> _hitScanBP;

	UPROPERTY()
	class ACAS_HitScan* _hitScan = nullptr;
};
