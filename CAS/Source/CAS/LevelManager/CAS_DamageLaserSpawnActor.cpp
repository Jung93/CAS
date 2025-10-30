// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_DamageLaserSpawnActor.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_PlayerState.h"
#include "LevelManager/CAS_LaserComponent.h"

ACAS_DamageLaserSpawnActor::ACAS_DamageLaserSpawnActor()
{
}

void ACAS_DamageLaserSpawnActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACAS_DamageLaserSpawnActor::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	//
	//FHitResult HitResult;
	//FVector Start = GetActorLocation();
	//FVector ForwardDir = GetActorForwardVector();
	//
	//LaserEnd += ForwardDir * Offset * DeltaTime;
	//
	//
	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	HitResult,
	//	Start,
	//	LaserEnd,
	//	ECC_GameTraceChannel7,
	//	FCollisionQueryParams(NAME_None, true, this)
	//);
	//
	//NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	//
	//if (bHit)
	//{
	//	LaserEnd = HitResult.ImpactPoint;
	//
	//	ACAS_Player* HitPlayer = Cast<ACAS_Player>(HitResult.GetActor());
	//
	//	if (HitPlayer)
	//	{
	//		if (CurrentHitPlayer != HitPlayer)
	//		{
	//			ClearContinuousDamageTimer();
	//
	//			CurrentHitPlayer = HitPlayer;
	//
	//			HitPlayer->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.TakeDamage"));
	//
	//			GetWorldTimerManager().SetTimer(
	//				ContinuousDamageTimerHandle,
	//				this,
	//				&ACAS_DamageLaserSpawnActor::ApplyContinuousDamage,
	//				DamageInterval,
	//				true
	//			);
	//		}
	//	}
	//	else
	//	{
	//		ClearContinuousDamageTimer();
	//		CurrentHitPlayer = nullptr;
	//	}
	//}
	//else
	//{
	//	ClearContinuousDamageTimer();
	//	CurrentHitPlayer = nullptr;
	//}
	////auto DetectedActor = LaserComponent->GetDetectedActor();
	////
	////if (auto Player = Cast<ACAS_Player>(DetectedActor)) {
	////	CurrentHitPlayer = Player;
	////
	////}
	////else {
	////	ClearContinuousDamageTimer();
	////}
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