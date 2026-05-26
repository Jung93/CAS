// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_Player.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Controller/CAS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Character/CAS_EnemyCapt.h"
#include "Character/CAS_Hat.h"
#include "Character/CAS_HitScan.h"
#include "Character/CAS_PlayerState.h"
#include "Character/NPC/CAS_SaveNPC.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "InteractionActor/CAS_InteractionCube.h"

#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "UI/CAS_QuickSlotWidget.h"
#include "UI/CAS_SelectSkillWidget.h"
#include "UI/CAS_TitleWidget.h"
#include "UI/CAS_Hpbar.h"

#include "Global/CAS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"


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
	//기본 어빌리티 등록
	static ConstructorHelpers::FClassFinder<UGameplayAbility> CaptureAbilityClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/Ability/GA_Ability_Capture.GA_Ability_Capture_C'"));
	static ConstructorHelpers::FClassFinder<UGameplayAbility> PickAbilityClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/Ability/GA_Ability_PickUp.GA_Ability_PickUp_C'"));
	static ConstructorHelpers::FClassFinder<UGameplayAbility> PushAbilityClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/Ability/GA_Ability_Push.GA_Ability_Push_C'"));

	if (CaptureAbilityClass.Succeeded())
	{
		DefaultAbilities.Add(CaptureAbilityClass.Class);
	}

	if (PickAbilityClass.Succeeded())
	{
		DefaultAbilities.Add(PickAbilityClass.Class);
	}

	if (PushAbilityClass.Succeeded())
	{
		DefaultAbilities.Add(PushAbilityClass.Class);
	}

	QuickSlotWidgetComponent = CreateDefaultSubobject<UCAS_QuickSlotWidgetComponent>(TEXT("QuickSlotWidgetComponent"));
	
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(GetRootComponent());
	WidgetInteraction->InteractionDistance = 1000.0f;
	WidgetInteraction->bShowDebug = true;
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;
	WidgetInteraction->TraceChannel = ECollisionChannel::ECC_GameTraceChannel8;
}

void ACAS_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2000.0f;
}

void ACAS_Player::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr) return;

	FVector ForwardDirection;
	FVector RightDirection;

	if (Cast<ACAS_InteractionCube>(InteractingActor))
	{
		// 캐릭터 정면 기준 이동
		ForwardDirection = GetActorForwardVector();
		RightDirection = FVector::ZeroVector; 

		// S 입력 무시
		if (MovementVector.Y < 0.0f)
			MovementVector.Y = 0.0f;

		MovementVector.X = 0.0f; 
	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}

	AddMovementInput(ForwardDirection, MovementVector.Y);

	if (!RightDirection.IsZero())
	{
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

void ACAS_Player::Jump()
{
	if(!IsInteracting)
		Super::Jump();

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
	//GameplayTag를 확인하여 해당 어빌리티 실행 
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.Capture"));
}

void ACAS_Player::RightMouseClicked(const FInputActionValue& Value)
{
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void ACAS_Player::RightMouseReleased(const FInputActionValue& Value)
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
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
	if (WidgetInteraction)
	{
		WidgetInteraction->bEnableHitTesting = true;
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
	if (WidgetInteraction)
	{
		WidgetInteraction->bEnableHitTesting = false;
	}
}

void ACAS_Player::QuickSlotFunction(const FInputActionValue& Value)
{
	if (_hatSpawn->GetIsThrowing())
		return;


	FCAS_SlotData SlotData = QuickSlotWidgetComponent->GetAbilityData(0);

	EAbilityInputID InputID = SlotData.SkillData.InputID;
	FName name = SlotData.SkillData.AbilityTag;

	if (name == "None")
		return;

	if (_hatSpawn->GetIsReady())
		return;

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

void ACAS_Player::ChangeSlot01(const FInputActionValue& Value)
{
	if (!isSlotChangable)
		return;

	//스킬 퀵슬록 로테이션 
	QuickSlotWidget->ChangeSlotToLeft();

	//퀵슬록에 등록된 어빌리티 순서 변경
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
	if (!isSlotChangable)
		return;
	//스킬 퀵슬록 로테이션 
	QuickSlotWidget->ChangeSlotToRight();

	//퀵슬록에 등록된 어빌리티 순서 변경
	int32 num = PlayerAbilityCount - 1;
	TArray<FCAS_SlotData>& abilities = QuickSlotWidgetComponent->GetPlayerAbilities();
	FCAS_SlotData data = abilities[num];

	for (int32 i = num; i > 0; i--)
	{
		abilities[i] = abilities[i - 1];
	}

	abilities[0] = data;
}

void ACAS_Player::InteractionInput(const FInputActionValue& Value)
{
	if (IsInteracting && InteractingActor)
	{
		if(Cast<ACAS_InteractionCube>(InteractingActor))
			ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Push"));
		else
			ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Pick"));

	}


	auto controller = GetController();
	if (!controller) {
		return;
	}
	auto PlayerController = Cast<ACAS_PlayerController>(controller);

	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel6, FCollisionShape::MakeSphere(88.0f), Params);
	
	if (!bHit || !HitResult.GetActor()) {
		return;
	}
	
	ACAS_InteractionActor* InteractionActor = Cast<ACAS_InteractionActor>(HitResult.GetActor());

	if (!InteractionActor) {
		return;
	}

	FVector2D ViewPortSize;
	GEngine->GameViewport->GetViewportSize(ViewPortSize);

	FVector2D ScreenPosition;
	PlayerController->ProjectWorldLocationToScreen(InteractionActor->GetActorLocation(), ScreenPosition);
	
	bool bIsInViewPort = ScreenPosition.X >= 0 && ScreenPosition.X <= ViewPortSize.X && ScreenPosition.Y >= 0 && ScreenPosition.Y <= ViewPortSize.Y;
	
	if (!bIsInViewPort) {
		return;
	}

	if (InteractionActor->CanInteraction()) {
		InteractionActor->InteractionWithPlayer();
	}
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

	if (_hitScanBP->IsValidLowLevel())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector Location = GetActorLocation();
		FRotator Rotation = FRotator::ZeroRotator;

		_hitScan = GetWorld()->SpawnActor<ACAS_HitScan>(_hitScanBP, Location, FRotator::ZeroRotator, SpawnParams);

		if (_hitScan)
		{
			_hitScan->SetOwnerClass(this);
			_hitScan->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("foot_r_Socket")); // 소켓 이름 "head" 예시
		}
	}

	auto NewQuickSlotWidget = CreateWidget<UCAS_QuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);
	auto NewSelectSkillWidget = CreateWidget<UCAS_SelectSkillWidget>(GetWorld(), SelectSkillWidgetClass);
	
	
	QuickSlotWidget = NewQuickSlotWidget;
	SelectSkillWidget = NewSelectSkillWidget;

	
	if (QuickSlotWidgetComponent->IsValidLowLevel() && QuickSlotWidget->IsValidLowLevel() && SelectSkillWidget->IsValidLowLevel()) {
		QuickSlotWidgetComponent->InitSetting(PlayerAbilityCount);
		QuickSlotWidget->AddToViewport();
		QuickSlotWidget->InitSetting(PlayerAbilityCount);
		SelectSkillWidget->InitSetting();
		QuickSlotWidget->QuickSlotSwapEvent.AddUObject(QuickSlotWidgetComponent, &UCAS_QuickSlotWidgetComponent::UpdateQuickSlot);
		QuickSlotWidget->RemoveAbilityEvent.AddUObject(QuickSlotWidgetComponent, &UCAS_QuickSlotWidgetComponent::RemovePlayerAbility);
		QuickSlotWidget->RemoveAbilityEvent.AddUObject(QuickSlotWidget, &UCAS_QuickSlotWidget::RemoveSlotData);
	}

	UCAS_GameInstance* GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());
	if (GameInstance) {
			GameInstance->ResetPuzzleClear();
		if (GameInstance->IsSaveDataReady()) {
			GameInstance->ApplyCachedGameData(this);
			GameInstance->ClearCachedData();
		}
		GameInstance->SetQuickSlotSize(PlayerAbilityCount);
		LoadCharacterData();		

	}
	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
	AttributeSet->DeadEvent.AddUObject(this, &ThisClass::DeactivateHat);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

	//타이틀 화면에서 플레이어 숨김
	if (CurrentLevelName == FName("Title"))
	{
		HideForTitle();
	}

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACAS_Player::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACAS_Player::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACAS_Player::Look);

		EnhancedInputComponent->BindAction(CaptureAction, ETriggerEvent::Started, this, &ACAS_Player::Capture);

		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ACAS_Player::RightMouseClicked);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &ACAS_Player::RightMouseReleased);

		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Started, this, &ACAS_Player::ShowMouse);
		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Completed, this, &ACAS_Player::HideMouse);

		EnhancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Started, this, &ACAS_Player::QuickSlotFunction);
		EnhancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Completed, this, &ACAS_Player::QuickSlotInputReleased);
		
		//EnhancedInputComponent->BindAction(OpenSlotAction, ETriggerEvent::Started, this, &ACAS_Player::OpenSlot);
		//EnhancedInputComponent->BindAction(OpenSlotAction, ETriggerEvent::Completed, this, &ACAS_Player::CloseSlot);

		EnhancedInputComponent->BindAction(ChangeSlotAction01, ETriggerEvent::Started, this, &ACAS_Player::ChangeSlot01);
		EnhancedInputComponent->BindAction(ChangeSlotAction02, ETriggerEvent::Started, this, &ACAS_Player::ChangeSlot02);
		EnhancedInputComponent->BindAction(IneractionAction, ETriggerEvent::Started, this, &ACAS_Player::InteractionInput);


	}
	
}

void ACAS_Player::DeactivateHat()
{
	//플리에어 사망 시 hat 숨김
	if (_hatSpawn) 
	{
		_hatSpawn->SetActorEnableCollision(false);
		_hatSpawn->SetActorHiddenInGame(true);
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

	if (playerState == nullptr)
		return nullptr;

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
	if (newAbility == nullptr) {
		return;
	}
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

bool ACAS_Player::IsDead()
{
	if (AttributeSet == nullptr)
		return false;

	return AttributeSet->GetHealth() <= 0;
}

void ACAS_Player::ToggleSkill()
{
	//토글, 지속형 스킬의 경우 스킬 아이콘 수정
	QuickSlotWidget->SwitchToggle();

}

void ACAS_Player::HideForTitle()
{
	SetActorHiddenInGame(true);

	if (_hatSpawn && QuickSlotWidget)
	{
		_hatSpawn->SetActorHiddenInGame(true);
		QuickSlotWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

//void ACAS_Player::AddDetectingEnemy(class ACAS_Character* enemy)
//{
//	if(DetectingEnemy.Find(enemy) < 0)
//		DetectingEnemy.Add(enemy);
//}
//
//void ACAS_Player::RemoveDetectingEnemy(class ACAS_Character* enemy)
//{
//	if(!DetectingEnemy.IsEmpty())
//		DetectingEnemy.Remove(enemy);
//}

void ACAS_Player::SaveCharacterData()
{
	Super::SaveCharacterData();

	auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());
	for(int32 i = 0; i<PlayerAbilityCount;i++){
		GameInstance->SetQuickSlotAbilityData(i, QuickSlotWidgetComponent->GetAbilityData(i));
	}
	auto currHP =GetAttributeSet()->GetHealth();
	GameInstance->SetPlayerHPCount(currHP);
}

void ACAS_Player::LoadCharacterData()
{
	Super::LoadCharacterData();

	auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());
	for (int32 i = 0; i < PlayerAbilityCount; i++) {
		FCAS_SlotData AbilityData = GameInstance->GetQuickSlotAbilityData(i);
		TSubclassOf<UGameplayAbility> AbilityClass = AbilityData.AbilityClass;
		AddPlayerAbility(AbilityClass);
	}

	auto CurrHP = GameInstance->GetPlayerHPCount();
	GameInstance->ClearPlayerHPCount();

	if (HpBarWidgetComponent) {
		auto widget = Cast<UCAS_Hpbar>(HpBarWidgetComponent->GetWidget());
		if (widget && CurrHP != -1) {
			widget->UpdateHp(CurrHP);
			GetAttributeSet()->SetHealth(CurrHP);
		}
	}
}
