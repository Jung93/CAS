// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_FireBreath.h"
#include "LevelManager/CAS_WorldSubsystem.h"

ACAS_EnemyCapt_FireBreath::ACAS_EnemyCapt_FireBreath()
{
}

void ACAS_EnemyCapt_FireBreath::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

	}
}

void ACAS_EnemyCapt_FireBreath::ActivateEnemyAbility()
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.FireBreath"));
}

void ACAS_EnemyCapt_FireBreath::BeginPlay()
{
	Super::BeginPlay();

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->RegisterTarget();
}
