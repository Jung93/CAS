// Fill out your copyright notice in the Description page of Project Settings.


#include "CAS_RockBig.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ACAS_RockBig::ACAS_RockBig()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");


	BoxCollider->SetupAttachment(Mesh);
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ACAS_RockBig::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ACAS_RockBig::OnPlayerOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ACAS_RockBig::OnPlayerOverlapEnd);
}

void ACAS_RockBig::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACAS_RockBig::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void ACAS_RockBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

