// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionCube.h"
#include "InteractionActor/CAS_InteractionBall.h"
#include "Character/CAS_Player.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

ACAS_InteractionCube::ACAS_InteractionCube()
{
	RootComponent = StaticMesh;

	SenseCollider->Deactivate();

	CubeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CubeCollider->SetCollisionProfileName(TEXT("InteractionSensor"));
	CubeCollider->SetupAttachment(StaticMesh);

}

void ACAS_InteractionCube::InteractionWithPlayer()
{
	if (PushPlyer)
	{
		auto ball = Cast<ACAS_InteractionBall>(PushPlyer->GetInteractingActor());

		if (ball != nullptr)
			return;

		PushPlyer->SetInteractingActor(this);
		PushPlyer->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Push"));


		FVector playerLocation = PushPlyer->GetActorLocation();

		FVector cubeLocation = GetActorLocation();
		//cubeLocation.Z = playerLocation.Z;

		FVector Direction = (cubeLocation - playerLocation).GetSafeNormal();

		FRotator NewRotation = Direction.Rotation();

		PushPlyer->SetActorRotation(NewRotation);



	}
}

void ACAS_InteractionCube::BeginPlay()
{
	Super::BeginPlay();

	CubeCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	CubeCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlapEvent);

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
