// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_TestCapture.h"

ACAS_TestCapture::ACAS_TestCapture()
{
}

void ACAS_TestCapture::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {


		// Moving
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::TestAbility);

	}
}

void ACAS_TestCapture::TestAbility(const FInputActionValue& Value)
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.TEST"));
}
