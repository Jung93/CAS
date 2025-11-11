// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionMirror.h"
#include "LevelManager/CAS_LaserComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CAS_KeyPressMovementUI.h"
#include "Character/CAS_Character.h"

ACAS_InteractionMirror::ACAS_InteractionMirror()
{
	PrimaryActorTick.bCanEverTick = true;

	LaserComponent = CreateDefaultSubobject<UCAS_LaserComponent>(TEXT("LaserComponent"));
	LaserComponent->SetupAttachment(StaticMesh);
}

void ACAS_InteractionMirror::InteractionWithPlayer()
{
	AddActorWorldRotation(FRotator(0, Degree, 0));
}

void ACAS_InteractionMirror::SetLaserActivated(bool LaserActivated)
{
	LaserComponent->SetLaserActivated(LaserActivated);
}

bool ACAS_InteractionMirror::GetLaserActivated()
{
	return LaserComponent->GetLaserActivated();
}
void ACAS_InteractionMirror::ResetMirrorTransform()
{
	SetActorTransform(InitTransform);
}

void ACAS_InteractionMirror::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetLocation.IsNearlyZero()) {
		FTransform ActorTransform = GetActorTransform();
		FVector WorldLocation = ActorTransform.TransformPosition(TargetLocation);

		FVector Dir = (WorldLocation - GetActorLocation()).GetSafeNormal();
		FRotator Rotator = Dir.Rotation();

		SetActorRotation(Rotator);
	}

	InitTransform = GetActorTransform();
		
	if (KeyPressUIClass) {		
		KeyPressWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		KeyPressWidgetComponent->SetVisibility(true);

	}
	KeyPressUI = Cast<UCAS_KeyPressMovementUI>(KeyPressWidgetComponent->GetUserWidgetObject());
	if (KeyTexture && KeyPressUI) {
		KeyPressUI->SetTexture(KeyTexture);
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ACAS_InteractionMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		if (auto Player = Cast<ACAS_Character>(PlayerController->GetPawn())) {

			FVector PlayerDir = Player->GetActorLocation() - KeyPressWidgetComponent->GetComponentLocation();
			PlayerDir.Z = 0;
			KeyPressWidgetComponent->SetWorldRotation(PlayerDir.Rotation());
		}
	}
}
