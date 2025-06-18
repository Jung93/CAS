// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_ChargeAttack.h"

ACAS_EnemyCapt_ChargeAttack::ACAS_EnemyCapt_ChargeAttack()
{
}

void ACAS_EnemyCapt_ChargeAttack::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto mesh = GetMesh();
	auto material = mesh->CreateDynamicMaterialInstance(0);
	auto material2 = mesh->CreateDynamicMaterialInstance(1);

	mesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue("Tint", FVector4(0.5f, 0.0f, 0.5f, 1.0f));
	mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Tint", FVector4(0.5f, 0.0f, 0.5f, 1.0f));
}
void ACAS_EnemyCapt_ChargeAttack::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::ActivateAbility);
	}

}

void ACAS_EnemyCapt_ChargeAttack::ActivateAbility(const FInputActionValue& Value)
{
	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.ChargeAttack"));

}
