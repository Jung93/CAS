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

	MovementUIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MovementUIWidgetComponent"));
	MovementUIWidgetComponent->SetupAttachment(StaticMesh);
}

void ACAS_InteractionMirror::InteractionWithPlayer()
{
	OpenMovementUIMode();
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
	
	if (MovementUIClass) {
		MovementUIWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		MovementUIWidgetComponent->SetWidgetClass(MovementUIClass);
		MovementUIWidgetComponent->SetVisibility(false);

		MovementUIWidgetComponent->SetRelativeLocation(FVector::ZeroVector);

		MovementUI = Cast<UCAS_KeyPressMovementUI>(MovementUIWidgetComponent->GetUserWidgetObject());

		if (MovementUI) {
			for (auto Info : UIInfo) {
				MovementUI->SetMovementTexture(Info.Texture, Info.Type);
			}
			MovementUI->SetVisibility(ESlateVisibility::Visible);
			MovementUI->WidgetClickEvent.AddUObject(this, &ThisClass::WidgetClicked);
		}
	}
	StaticMesh->SetRenderCustomDepth(false);
}

void ACAS_InteractionMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (auto PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (auto Player = Cast<ACAS_Character>(PlayerController->GetPawn()))
		{
			FVector CamLoc;
			FRotator CamRot;
			PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	
			FVector Dir = CamLoc - MovementUIWidgetComponent->GetComponentLocation();
			Dir.Z = 0;
			MovementUIWidgetComponent->SetWorldRotation(Dir.Rotation());
		}
	}
}

void ACAS_InteractionMirror::WidgetClicked(EWidgetPositionType Type)
{
	if (Type == EWidgetPositionType::NONE) {
		return;
	}
	else if (Type == EWidgetPositionType::Left) {
		StaticMesh->AddRelativeRotation(FRotator(0, -Degree, 0));
	}
	else if (Type == EWidgetPositionType::Right) {
		StaticMesh->AddRelativeRotation(FRotator(0, Degree, 0));
	}
}

void ACAS_InteractionMirror::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


}

void ACAS_InteractionMirror::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ExitMovementUIMode();
}

void ACAS_InteractionMirror::OpenMovementUIMode()
{
	KeyPressWidgetComponent->SetVisibility(false);
	MovementUIWidgetComponent->SetVisibility(true);
}

void ACAS_InteractionMirror::ExitMovementUIMode()
{
	KeyPressWidgetComponent->SetVisibility(true);
	MovementUIWidgetComponent->SetVisibility(false);
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
