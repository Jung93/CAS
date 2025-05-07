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

	void BeCaptured(class ACAS_Hat* hat);

	void Move(const FInputActionValue& Value);
	void TestDeCapture(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TestDeCaptureAction;

	UPROPERTY(VisibleAnywhere, Category = "Hat")
	class ACAS_Hat* _hat;

	bool _isCaptured = false;
};
