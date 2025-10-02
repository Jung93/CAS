// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyNoCapt.h"
#include "CAS_EnemyNoCapt_ScaredRunner.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyNoCapt_ScaredRunner : public ACAS_EnemyNoCapt
{
	GENERATED_BODY()
public:
	ACAS_EnemyNoCapt_ScaredRunner();
protected:
	virtual void BeginPlay() override;
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual class UCAS_AttributeSet* GetAttributeSet() const override { return AttributeSet; }
protected:
    UFUNCTION()
	void OverlapBeginEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
