// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_DamageLaserSpawnActor.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_PlayerState.h"
#include "LevelManager/CAS_LaserComponent.h"

ACAS_DamageLaserSpawnActor::ACAS_DamageLaserSpawnActor()
{

	PrimaryActorTick.bCanEverTick = true;

}

void ACAS_DamageLaserSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	SetLaserActivated(true);

}

void ACAS_DamageLaserSpawnActor::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	auto DetectedActor = LaserComponent->GetDetectedActor();
	auto Player = Cast<ACAS_Player>(DetectedActor);
	if (Player) {
		if (CurrentHitPlayer != Player)
		{
			ClearContinuousDamageTimer();

			CurrentHitPlayer = Player;

			Player->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.TakeDamage"));

			GetWorldTimerManager().SetTimer(
				ContinuousDamageTimerHandle,
				this,
				&ACAS_DamageLaserSpawnActor::ApplyContinuousDamage,
				DamageInterval,
				true
			);
		}
	}
	else
	{
		ClearContinuousDamageTimer();
		CurrentHitPlayer = nullptr;
	}
}

void ACAS_DamageLaserSpawnActor::ApplyContinuousDamage()
{
	if (CurrentHitPlayer)
	{
		CurrentHitPlayer->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.TakeDamage"));
	}
	else
	{
		ClearContinuousDamageTimer();
		CurrentHitPlayer = nullptr;
	}
}

void ACAS_DamageLaserSpawnActor::ClearContinuousDamageTimer()
{
	if (GetWorldTimerManager().IsTimerActive(ContinuousDamageTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(ContinuousDamageTimerHandle);
	}
}