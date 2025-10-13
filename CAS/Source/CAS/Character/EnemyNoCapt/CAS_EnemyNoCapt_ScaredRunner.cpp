// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyNoCapt/CAS_EnemyNoCapt_ScaredRunner.h"
#include "Components/CapsuleComponent.h"
#include "Character/CAS_Hat.h"
#include "Components/WidgetComponent.h"
#include "LevelManager/CAS_WorldSubsystem.h"

ACAS_EnemyNoCapt_ScaredRunner::ACAS_EnemyNoCapt_ScaredRunner()
{
}

void ACAS_EnemyNoCapt_ScaredRunner::BeginPlay()
{
	Super::BeginPlay();

	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.SuperSpeed"));
	HpBarWidgetComponent->SetVisibility(false);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapBeginEvent);
}

void ACAS_EnemyNoCapt_ScaredRunner::OverlapBeginEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bOverlap) {
		return;
	}
	auto Hat = Cast<ACAS_Hat>(OtherActor);

	if (!Hat) {
		return;
	}
	else {
		bOverlap = true;
		TakeDamageEvent();
		auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
		PuzzleSubsystem->StageClearEvent();
	}

}
