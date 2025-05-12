// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_EnemyCapt.h"
#include "Controller/CAS_PlayerController.h"
#include "Engine/LocalPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/CAS_Hat.h"
#include "AIController.h"
#include "CAS/Character/CAS_Character.h"
#include "CAS/Character/CAS_Player.h"

ACAS_EnemyCapt::ACAS_EnemyCapt()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

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

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACAS_EnemyCapt::Move);
		EnhancedInputComponent->BindAction(TestDeCaptureAction, ETriggerEvent::Started, this, &ACAS_EnemyCapt::TestDeCapture);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACAS_EnemyCapt::Look);

	}

}

void ACAS_EnemyCapt::BeCaptured(ACAS_Hat* hat)
{

	_hat = hat;
	_isCaptured = true;

	auto controller = GetController();

	controller->UnPossess();

	ACAS_PlayerController* playerController = Cast<ACAS_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		playerController->SetViewTargetWithBlend(this, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([playerController, this]() {
			playerController->Possess(this);
			}), 1.0f, false);

		FollowCamera->SetActive(true);

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
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

	auto controller = Cast<APlayerController>(GetController());
	controller->UnPossess();
	controller->Possess(_hat->GetPlayer());


	auto iter = GetWorld()->GetControllerIterator();
	for (int i = 0; i < GetWorld()->GetNumControllers(); i++)
	{
		if ((*iter)->GetPawn() != nullptr)
			iter++;
		break;
	}

	auto aiController = Cast<AAIController>(*iter);
	aiController->Possess(this);

	_hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_hat->Return();
	_hat = nullptr;

}

void ACAS_EnemyCapt::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACAS_EnemyCapt::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	if (FollowCamera)
	{
		FollowCamera->GetCameraView(DeltaTime, OutResult);
	}
}
