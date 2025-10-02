// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyNoCapt/CAS_EnemyNoCapt_ScaredRunner.h"
#include "Components/CapsuleComponent.h"
ACAS_EnemyNoCapt_ScaredRunner::ACAS_EnemyNoCapt_ScaredRunner()
{
}

void ACAS_EnemyNoCapt_ScaredRunner::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapBeginEvent);
}

void ACAS_EnemyNoCapt_ScaredRunner::OverlapBeginEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
