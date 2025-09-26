// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionBall.h"
#include "Character/CAS_Player.h"

ACAS_InteractionBall::ACAS_InteractionBall()
{

	RootComponent = StaticMesh;
}

void ACAS_InteractionBall::InteractionWithPlayer()
{
	if (PickPlayer)
	{
		PickPlayer->SetInteractingActor(this);
		PickPlayer->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Pick"));
	}
}

void ACAS_InteractionBall::BeginPlay()
{
	Super::BeginPlay();
}

void ACAS_InteractionBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAS_InteractionBall::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	auto OverlapPlayer = Cast<ACAS_Player>(OtherActor);

	if (OverlapPlayer)
		PickPlayer = OverlapPlayer;
}

void ACAS_InteractionBall::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	auto OverlapPlayer = Cast<ACAS_Player>(OtherActor);

	if (OverlapPlayer)
		PickPlayer = nullptr;
}
