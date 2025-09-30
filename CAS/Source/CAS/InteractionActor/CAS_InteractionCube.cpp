// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionCube.h"
#include "Character/CAS_Player.h"


ACAS_InteractionCube::ACAS_InteractionCube()
{
	RootComponent = StaticMesh;
}

void ACAS_InteractionCube::InteractionWithPlayer()
{
	if (PushPlyer)
	{
		PushPlyer->SetInteractingActor(this);
		PushPlyer->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Push"));
	}
}

void ACAS_InteractionCube::BeginPlay()
{
	Super::BeginPlay();
}

void ACAS_InteractionCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAS_InteractionCube::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	auto OverlapPlayer = Cast<ACAS_Player>(OtherActor);

	if (OverlapPlayer)
		PushPlyer = OverlapPlayer;
}

void ACAS_InteractionCube::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	auto OverlapPlayer = Cast<ACAS_Player>(OtherActor);

	if (OverlapPlayer)
		PushPlyer = nullptr;
}
