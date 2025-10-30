// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_LaserComponent.h"
#include "LevelManager/CAS_LaserTarget.h"
#include "InteractionActor/CAS_InteractionMirror.h"

// Sets default values for this component's properties
UCAS_LaserComponent::UCAS_LaserComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCAS_LaserComponent::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComponent->SetAsset(NiagaraSystem);
	NiagaraComponent->SetVariableLinearColor(TEXT("Color"), FLinearColor(0.8f, 0, 0.7f, 1.0f));
	NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), GetOwner()->GetActorLocation());
	NiagaraComponent->Activate();
}

void UCAS_LaserComponent::TraceLaser(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) {
		return;
	}

	FHitResult HitResult;
	FVector StartPosition = GetOwner()->GetActorLocation();
	FVector ForwardDir = GetOwner()->GetActorForwardVector();

	FVector TargetEnd = StartPosition + ForwardDir * Offset;
	LaserEnd = FMath::VInterpTo(LaserEnd, TargetEnd, DeltaTime, 10.0f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPosition,
		LaserEnd,
		ECC_GameTraceChannel7,
		FCollisionQueryParams(NAME_None, true, Owner)
	);

	if (bHit)
	{
		LaserEnd = HitResult.ImpactPoint;
		ChildActor = HitResult.GetActor();

		if(auto Target = Cast<ACAS_LaserTarget>(HitResult.GetActor())) {
			Target->LaserReached();
		}
		else if (auto Mirror = Cast<ACAS_InteractionMirror>(HitResult.GetActor())) {
			if (Mirror != ChildActor) {
				ClearChildActor();
				Mirror->SetLaserActivated(true);
			}
		}
	}
	else {
		if (ChildActor) {
			ClearChildActor();
		}
	}

	NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
}

void UCAS_LaserComponent::ClearChildActor()
{
	if (auto Mirror = Cast<ACAS_InteractionMirror>(ChildActor)) {
		Mirror->SetLaserActivated(false);
	}
	ChildActor = nullptr;
}


// Called every frame
void UCAS_LaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bLaserActivated) {
		TraceLaser(DeltaTime);
		NiagaraComponent->Activate();
	}
	else {
		ClearChildActor();
		NiagaraComponent->Deactivate();
		LaserEnd = GetOwner()->GetActorLocation();
		NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	}
}

AActor* UCAS_LaserComponent::GetDetectedActor()
{
	if (HasLaserHitChildActor()) {
		return ChildActor;
	}

	return nullptr;
}
