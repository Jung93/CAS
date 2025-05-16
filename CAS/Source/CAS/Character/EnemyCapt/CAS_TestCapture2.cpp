// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_TestCapture2.h"

ACAS_TestCapture2::ACAS_TestCapture2()
{
}

void ACAS_TestCapture2::BeginPlay()
{
	Super::BeginPlay();

	//auto mesh = GetMesh();
	//auto material = mesh->CreateDynamicMaterialInstance(0);
	//auto material2 = mesh->CreateDynamicMaterialInstance(1);

	//mesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue("Tint", FVector4(1,0,0,1));
	//mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Tint", FVector4(1,0,0,1));
}

void ACAS_TestCapture2::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto mesh = GetMesh();
	auto material = mesh->CreateDynamicMaterialInstance(0);
	auto material2 = mesh->CreateDynamicMaterialInstance(1);

	mesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue("Tint", FVector4(1,0,0,1));
	mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Tint", FVector4(1,0,0,1));


}

void ACAS_TestCapture2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::TestAbility);

	}
}

void ACAS_TestCapture2::TestAbility(const FInputActionValue& Value)
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.Evade"));
}
