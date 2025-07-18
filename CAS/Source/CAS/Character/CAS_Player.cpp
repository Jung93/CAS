// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_Player.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Controller/CAS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Character/CAS_EnemyCapt.h"
#include "Character/CAS_Hat.h"
#include "Character/CAS_PlayerState.h"

#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "UI/CAS_QuickSlotWidget.h"
#include "UI/CAS_SelectSkillWidget.h"





// Sets default values
ACAS_Player::ACAS_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
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

	AbilitySystemComponent = nullptr;
	AttributeSet = nullptr;
	static ConstructorHelpers::FClassFinder<UGameplayAbility> CaptureAbilityClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/Ability/GA_Ability_Capture.GA_Ability_Capture_C'"));


	if (CaptureAbilityClass.Succeeded())
	{
		DefaultAbilities.Add(CaptureAbilityClass.Class);
	}

	QuickSlotWidgetComponent = CreateDefaultSubobject<UCAS_QuickSlotWidgetComponent>(TEXT("QuickSlotWidgetComponent"));


}

void ACAS_Player::Move(const FInputActionValue& Value)
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

void ACAS_Player::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACAS_Player::StealAbility(const FInputActionValue& Value)
{
	FVector2D viewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(viewportSize);
	}
	
	bool isPressed = Value.Get<bool>();
	auto controller = Cast<ACAS_PlayerController>(GetController());
	if (controller != nullptr && isPressed) {
		auto screenX = viewportSize.X / 2.0f;
		auto screenY = viewportSize.Y / 2.0f;
	
		FVector WorldLocation;
		FVector WorldDirection;
	
		if (controller->DeprojectScreenPositionToWorld(screenX, screenY, WorldLocation, WorldDirection)) {
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 1000.0f);
	
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("CAS_line")), true, this);
			TraceParams.bTraceComplex = true;
			TraceParams.bReturnPhysicalMaterial = false;
	
			DrawDebugLine(
				GetWorld(),
				Start,
				End,
				FColor::Red,   
				false,         
				3.0f,          
				0,             
				5.0f           
			);


			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				Start,
				End,
				ECC_GameTraceChannel1,
				TraceParams
			);
			if (bHit)
			{
				AActor* HitActor = HitResult.GetActor();
				if (HitActor)
				{
					auto Enemy = Cast<ACAS_EnemyCapt>(HitActor);
					
					if (Enemy->IsValidLowLevel()) {
						Enemy->AddPlayerAbility(this);
					}
					
				}
			}
		}
	
	}
}

void ACAS_Player::Capture(const FInputActionValue& Value)
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.Capture"));

}

void ACAS_Player::ShowMouse(const FInputActionValue& Value)
{
	auto controller = Cast<ACAS_PlayerController>(GetController());

	if (controller->IsValidLowLevel())
	{
		controller->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		controller->SetInputMode(InputMode);
	}
}

void ACAS_Player::HideMouse(const FInputActionValue& Value)
{
	auto controller = Cast<ACAS_PlayerController>(GetController());

	if (controller->IsValidLowLevel())
	{
		controller->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		controller->SetInputMode(InputMode);
	}
}

void ACAS_Player::QuickSlotFunction(const FInputActionValue& Value)
{
	FCAS_SlotData SlotData = QuickSlotWidgetComponent->GetAbilityData(0);

	EAbilityInputID InputID = SlotData.SkillData.InputID;
	FName name = SlotData.SkillData.AbilityTag;

	ActivateAbility(FGameplayTag::RequestGameplayTag(name));

	switch (InputID) {
	case EAbilityInputID::None:
		break;
	case EAbilityInputID::ChargeAttack:
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));
		break;
	}
	
}

void ACAS_Player::QuickSlotInputReleased(const FInputActionValue& Value)
{
	FCAS_SlotData SlotData = QuickSlotWidgetComponent->GetAbilityData(0);

	EAbilityInputID InputID = SlotData.SkillData.InputID;

	switch (InputID) {
	case EAbilityInputID::None:
		break;
	case EAbilityInputID::ChargeAttack:
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(InputID));
		break;
	}
	
}


//void ACAS_Player::OpenSlot(const FInputActionValue& Value)
//{
//	QuickSlotWidget->OpenSlot();
//
//}
//
//void ACAS_Player::CloseSlot(const FInputActionValue& Value)
//{
//	QuickSlotWidget->CloseSlot();
//
//}

void ACAS_Player::ChangeSlot01(const FInputActionValue& Value)
{

	QuickSlotWidget->ChangeSlotToLeft();

	int32 num = PlayerAbilityCount - 1;
	TArray<FCAS_SlotData>& abilities = QuickSlotWidgetComponent->GetPlayerAbilities();
	FCAS_SlotData data = abilities[0];

	for (int32 i = 0; i < num; i++)
	{
		abilities[i] = abilities[i + 1];
	}

	abilities[num] = data;

}

void ACAS_Player::ChangeSlot02(const FInputActionValue& Value)
{
	QuickSlotWidget->ChangeSlotToRight();

	int32 num = PlayerAbilityCount - 1;
	TArray<FCAS_SlotData>& abilities = QuickSlotWidgetComponent->GetPlayerAbilities();
	FCAS_SlotData data = abilities[num];

	for (int32 i = num; i > 0; i--)
	{
		abilities[i] = abilities[i - 1];
	}

	abilities[0] = data;
}

// Called when the game starts or when spawned
void ACAS_Player::BeginPlay()
{
	Super::BeginPlay();

	if (_hatBP->IsValidLowLevel())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector Location = GetActorLocation() + FVector(0, 0, 50.0f);
		FRotator Rotation = FRotator::ZeroRotator;

		_hatSpawn = GetWorld()->SpawnActor<ACAS_Hat>(_hatBP, Location, FRotator::ZeroRotator, SpawnParams);

		if (_hatSpawn)
		{

			_hatSpawn->SetPlayer(this);
			_hatSpawn->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시
		}
	}
	auto NewQuickSlotWidget = CreateWidget<UCAS_QuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);

	QuickSlotWidget = NewQuickSlotWidget;

	if (QuickSlotWidgetComponent->IsValidLowLevel() && QuickSlotWidget->IsValidLowLevel()) {
		QuickSlotWidgetComponent->InitSetting(PlayerAbilityCount);
		QuickSlotWidget->AddToViewport();
		QuickSlotWidget->InitSetting(PlayerAbilityCount);
		QuickSlotWidget->QuickSlotSwapEvent.AddUObject(QuickSlotWidgetComponent, &UCAS_QuickSlotWidgetComponent::UpdateQuickSlot);
		QuickSlotWidget->RemoveAbilityEvent.AddUObject(QuickSlotWidgetComponent, &UCAS_QuickSlotWidgetComponent::RemovePlayerAbility);
		QuickSlotWidget->RemoveAbilityEvent.AddUObject(QuickSlotWidget, &UCAS_QuickSlotWidget::RemoveSlotData);
	}

	auto NewSelectSkillWidget = CreateWidget<UCAS_SelectSkillWidget>(GetWorld(), SelectSkillWidgetClass);

	SelectSkillWidget = NewSelectSkillWidget;

	if (SelectSkillWidget->IsValidLowLevel())
	{
		SelectSkillWidget->InitSetting();
	}

	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
}

// Called every frame
void ACAS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACAS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACAS_Player::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACAS_Player::Look);

		EnhancedInputComponent->BindAction(CaptureAction, ETriggerEvent::Started, this, &ACAS_Player::Capture);
		//EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ACAS_Player::StealAbility);

		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Started, this, &ACAS_Player::ShowMouse);
		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Completed, this, &ACAS_Player::HideMouse);

		EnhancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Started, this, &ACAS_Player::QuickSlotFunction);
		EnhancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Completed, this, &ACAS_Player::QuickSlotInputReleased);
		
		//EnhancedInputComponent->BindAction(OpenSlotAction, ETriggerEvent::Started, this, &ACAS_Player::OpenSlot);
		//EnhancedInputComponent->BindAction(OpenSlotAction, ETriggerEvent::Completed, this, &ACAS_Player::CloseSlot);

		EnhancedInputComponent->BindAction(ChangeSlotAction01, ETriggerEvent::Started, this, &ACAS_Player::ChangeSlot01);
		EnhancedInputComponent->BindAction(ChangeSlotAction02, ETriggerEvent::Started, this, &ACAS_Player::ChangeSlot02);


	}
	
}

void ACAS_Player::InitAbilitySystemComponent()
{
	ACAS_PlayerState* playerState = GetPlayerState<ACAS_PlayerState>();
	if (!playerState) {
		FString DebugMessage = FString::Printf(TEXT("PlayerState XXXXXXX"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, DebugMessage);
		return; 
	}

	AbilitySystemComponent = Cast<UCAS_AbilitySystemComponent>(playerState->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(playerState, this);
	AttributeSet = playerState->GetAttributeSet();
}

UAbilitySystemComponent* ACAS_Player::GetAbilitySystemComponent() const
{
	ACAS_PlayerState* playerState = GetPlayerState<ACAS_PlayerState>();
	auto AbilitySystemComp = playerState->GetAbilitySystemComponent();

	return AbilitySystemComp;
}

UCAS_AttributeSet* ACAS_Player::GetAttributeSet() const
{
	ACAS_PlayerState* playerState = GetPlayerState<ACAS_PlayerState>();
	auto Attribute = playerState->GetAttributeSet();

	return Attribute;
}

void ACAS_Player::AddPlayerAbility(TSubclassOf<class UGameplayAbility> newAbility)
{
	bool CanAddable = QuickSlotWidgetComponent->CheckPlayerAbility(newAbility);

	if (!CanAddable)
		return;

	int32 Index = QuickSlotWidgetComponent->FindEmptyPlayerAbilityIndex();
	if (Index < 0) {
		SelectSkillWidget->SetSlots(QuickSlotWidget->GetSkillSlots(), newAbility);
		QuickSlotWidget->BlockSlotSwap(SelectSkillWidget->GetSkillSlots());

		return;
	}

	QuickSlotWidgetComponent->AddPlayerAbility(Index, newAbility);
	auto SlotData = QuickSlotWidgetComponent->GetAbilityData(Index);
	QuickSlotWidget->SetSlotData(Index, SlotData);
}
