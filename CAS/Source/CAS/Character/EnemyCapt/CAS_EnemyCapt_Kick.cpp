// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_Kick.h"
#include "Character/CAS_HitScan.h"
#include "LevelManager/CAS_WorldSubsystem.h"

ACAS_EnemyCapt_Kick::ACAS_EnemyCapt_Kick()
{
}

void ACAS_EnemyCapt_Kick::ActivateEnemyAbility()
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.Test"));
}

void ACAS_EnemyCapt_Kick::BeginPlay()
{
	Super::BeginPlay();

	if (_hitScanBP->IsValidLowLevel())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector Location = GetActorLocation();
		FRotator Rotation = FRotator::ZeroRotator;

		_hitScan = GetWorld()->SpawnActor<ACAS_HitScan>(_hitScanBP, Location, FRotator::ZeroRotator, SpawnParams);

		if (_hitScan)
		{
			_hitScan->SetOwnerClass(this);
			_hitScan->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("foot_r_Socket")); // 소켓 이름 "head" 예시
		}
	}

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->RegisterTarget();

}

//void ACAS_EnemyCapt_Kick::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
//
//		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::ActivateAbility);
//	}
//
//}
//
//void ACAS_EnemyCapt_Kick::ActivateAbility(const FInputActionValue& Value)
//{
//	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.Test"));
//}
