// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyNoCapt/CAS_EnemyNoCapt_ScaredRunner.h"
#include "Components/CapsuleComponent.h"
#include "Character/CAS_Hat.h"

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
	auto Hat = Cast<ACAS_Hat>(OtherActor);

	if (!Hat) {
		return;
	}
	//TODO : 모자에 겹쳐지면 열쇠를 얻는다던지 해서 다음 맵이나 퍼즐이 열리도록
}
