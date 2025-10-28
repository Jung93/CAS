// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_BallTargetPoint.h"
#include "Components/BoxComponent.h"
#include "InteractionActor/CAS_InteractionBall.h"
#include "LevelManager/CAS_WorldSubsystem.h"

// Sets default values
ACAS_BallTargetPoint::ACAS_BallTargetPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BallDetectCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

	RootComponent = StaticMesh;
	BallDetectCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACAS_BallTargetPoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (BallDetectCollider) {
		BallDetectCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
		BallDetectCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlapEvent);
		auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
		PuzzleSubsystem->RegisterTarget();
	}

	auto Material = StaticMesh->GetMaterial(0);

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

		StaticMesh->SetMaterial(0, DynamicMaterial);

		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
	}

}


void ACAS_BallTargetPoint::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto InteractionBall = Cast<ACAS_InteractionBall>(OtherActor)) {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.6f, 0.0f, 1.0f));

		bTargetInCollider = true;
		if (!bCheckOnce) {
			GetWorldTimerManager().SetTimer(CheckTimerHandle, this, &ThisClass::CheckTarget, 0.1f, true);
		}
		else {
			auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
			PuzzleSubsystem->PlusCompletedCount();
		}
	}
	else {
		return;
	}

}

void ACAS_BallTargetPoint::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bCheckOnce) {
		return;
	}
	if (auto InteractionBall = Cast<ACAS_InteractionBall>(OtherActor)) {
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
		
		bTargetInCollider = false;
	}
	else {
		return;
	}

}

void ACAS_BallTargetPoint::CheckTarget()
{
	if (bTargetInCollider) {
		CheckTime += 0.1f;
	}
	else {
		CheckTime = 0.0f;
	}
	
	if (CheckTime >= ClearTime) {
		GetWorldTimerManager().ClearTimer(CheckTimerHandle);
		auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
		PuzzleSubsystem->PlusCompletedCount();
	}
}
