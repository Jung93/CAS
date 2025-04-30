// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_Character.h"
#include "CAS_EnemyCapt.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt : public ACAS_Character
{
	GENERATED_BODY()
	

public:
	ACAS_EnemyCapt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual class UCAS_AttributeSet* GetAttributeSet() const override { return AttributeSet; }
};
