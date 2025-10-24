// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserSpawnActor.h"
#include "InteractionActor/CAS_InteractionMirror.h"
// Sets default values
ACAS_LaserSpawnActor::ACAS_LaserSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));

	SetRootComponent(StaticMesh);

	NiagaraComponent->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ACAS_LaserSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRotation(InitDir.Rotation());

	FVector StartPosition = GetActorLocation();
	FVector ForwardDir = GetActorForwardVector();
	LaserEnd = StartPosition + ForwardDir * Offset;

	NiagaraComponent->SetAsset(NiagaraSystem);
	NiagaraComponent->SetVariableLinearColor(TEXT("Color"), FLinearColor(0.8f, 0, 0.7f, 1.0f));
	NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	NiagaraComponent->Activate();
}

// Called every frame
void ACAS_LaserSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector Start = GetActorLocation();          
	FVector ForwardDir = GetActorForwardVector();

	LaserEnd += ForwardDir * Offset * DeltaTime;
	

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		LaserEnd,
		ECC_GameTraceChannel7,
		FCollisionQueryParams(NAME_None, true, this)
	);

	if (bHit)
	{
		LaserEnd = HitResult.ImpactPoint;

		if (auto Mirror = Cast<ACAS_InteractionMirror>(HitResult.GetActor())) {
			Mirror->LaserActivated(true);
		}
	}

	NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
}
