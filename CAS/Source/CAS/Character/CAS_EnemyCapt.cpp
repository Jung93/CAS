// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_EnemyCapt.h"
#include "Controller/CAS_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/CAS_Hat.h"

ACAS_EnemyCapt::ACAS_EnemyCapt()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FollowCamera->SetActive(false);
}

void ACAS_EnemyCapt::BeginPlay()
{
	Super::BeginPlay();
}

void ACAS_EnemyCapt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAS_EnemyCapt::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACAS_EnemyCapt::Move);
		EnhancedInputComponent->BindAction(TestDeCaptureAction, ETriggerEvent::Started, this, &ACAS_EnemyCapt::TestDeCapture);
		
	}

}

void ACAS_EnemyCapt::BeCaptured(ACAS_Hat* hat)
{

	_hat = hat;
	_isCaptured = true;

	auto controller = GetController();

	controller->UnPossess();
	//controller->Destroy();

	ACAS_PlayerController* playerController = Cast<ACAS_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		playerController->Possess(this);
		UE_LOG(LogTemp, Warning, TEXT("posses"));
		FollowCamera->SetActive(true);
	}

}

void ACAS_EnemyCapt::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACAS_EnemyCapt::TestDeCapture(const FInputActionValue& Value)
{
	_isCaptured = false;

	auto controller = GetController();

	controller->UnPossess();

	_hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_hat->Return();

	_hat = nullptr;
}
