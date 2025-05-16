// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_TestCapture2.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_TestCapture2 : public ACAS_EnemyCapt
{
	GENERATED_BODY()
	
public:
	ACAS_TestCapture2();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TestAbility(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;


	UPROPERTY(EditAnywhere, Category = "Color")
	FVector4 _color;

};
