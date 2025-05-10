// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_TestCapture.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_TestCapture : public ACAS_EnemyCapt
{
	GENERATED_BODY()
	
public:
	ACAS_TestCapture();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TestAbility(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;
};
