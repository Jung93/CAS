// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionMirror.h"

ACAS_InteractionMirror::ACAS_InteractionMirror()
{
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));

	NiagaraComponent->SetupAttachment(StaticMesh);

}

void ACAS_InteractionMirror::InteractionWithPlayer()
{
	AddActorLocalRotation(FRotator(0, Degree, 0));
}

void ACAS_InteractionMirror::RegisterParent(AActor* Parent)
{
	ParentLaser = Parent;
	bLaserActivated = true;
}

void ACAS_InteractionMirror::CancelRegistration()
{
	if (ParentLaser) {
		ParentLaser = nullptr;
	}
	bLaserActivated = false;
}

void ACAS_InteractionMirror::BeginPlay()
{
	Super::BeginPlay();

	FVector StartPosition = GetActorLocation();
	FVector ForwardDir = GetActorForwardVector();
	LaserEnd = StartPosition + ForwardDir * Offset;

	NiagaraComponent->SetAsset(NiagaraSystem);
	NiagaraComponent->SetVariableLinearColor(TEXT("Color"), FLinearColor(0.8f, 0, 0.7f, 1.0f));
	NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	NiagaraComponent->Deactivate();
}

void ACAS_InteractionMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bLaserActivated) {
		if (ChildLaser) {
			ChildLaser->CancelRegistration();
			ChildLaser = nullptr;
		}
		NiagaraComponent->Deactivate();
		LaserEnd = GetActorLocation();
		NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	}
	else {
		NiagaraComponent->Activate();

		FHitResult HitResult;
		FVector Start = GetActorLocation();
		FVector ForwardDir = GetActorForwardVector();

		FVector TargetEnd = Start + ForwardDir * Offset;
		LaserEnd = FMath::VInterpTo(LaserEnd, TargetEnd, DeltaTime, 10.0f);


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
				if (!ChildLaser) {
					ChildLaser = Mirror;
				}
				else if (ChildLaser != Mirror) {
					ChildLaser->CancelRegistration();
					ChildLaser = Mirror;
					ChildLaser->RegisterParent(this);
				}
				else {
					ChildLaser = Mirror;
					ChildLaser->RegisterParent(this);
				}
			}
			else {
				if (ChildLaser) {
					ChildLaser->CancelRegistration();
					ChildLaser = nullptr;
				}
			}
		}
		else {
			if (ChildLaser) {
				ChildLaser->CancelRegistration();
				ChildLaser = nullptr;
			}
		}

		NiagaraComponent->SetVariableVec3(TEXT("LaserEnd"), LaserEnd);
	}
}
