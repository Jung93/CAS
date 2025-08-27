// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_Hat.h"
#include "Components/SphereComponent.h"
#include "CAS/Character/CAS_EnemyCapt.h"
#include "CAS/Character/CAS_Player.h"

// Sets default values
ACAS_Hat::ACAS_Hat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	_collider->SetupAttachment(_mesh);
	RootComponent = _mesh;
}

// Called when the game starts or when spawned
void ACAS_Hat::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACAS_Hat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isThrowing)
	{
		ThrowAndReturn(DeltaTime);
	}

}

void ACAS_Hat::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_collider->OnComponentBeginOverlap.AddDynamic(this, &ACAS_Hat::OnMyCharacterOverlap);
 
}

// Called to bind functionality to input
void ACAS_Hat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACAS_Hat::Ready()
{
	IsReady = true;
	AttachToComponent(_player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("index_02_r")); // 소켓 이름 "head" 예시
}

void ACAS_Hat::OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	if (!_isThrowing)
		return;

	auto enemy = Cast<ACAS_EnemyCapt>(OtherActor);


	if (enemy->IsValidLowLevel())
	{
		auto asc = enemy->GetAbilitySystemComponent();
		auto tag = FGameplayTag::RequestGameplayTag(FName("Effect.Status.Stun"));

		if (asc->HasMatchingGameplayTag(tag))
			return;

		_testCaptureTarget = enemy;
		_testCaptureTarget->BeCaptured(this);

		_isThrowing = false;
		_isReturning = false;

		AttachToComponent(_testCaptureTarget->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시

		_player->SetActorHiddenInGame(true);
		_player->SetActorEnableCollision(false);
		return;
	}

	if (_isReturning)
	{
		auto player = Cast<ACAS_Player>(OtherActor);

		if (player->IsValidLowLevel())
		{

			_isThrowing = false;
			_isReturning = false;

			AttachToComponent(_player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시

		}
	}

}

void ACAS_Hat::Throw(const FVector& direction)
{
	//if (_isThrowing)
	//	return;

	if (_testCaptureTarget->IsValidLowLevel())
		return;

	StartLocation = GetActorLocation();
	MoveDirection = direction.GetSafeNormal();
	TargetLocation = StartLocation + MoveDirection * 600.0f;

	IsReady = false;
	_isThrowing = true;
	_capturingTime = 0.0f;
	_isReturning = false;
}

void ACAS_Hat::ThrowAndReturn(float DeltaTime)
{
	_capturingTime += DeltaTime;


	float halfTime = _isReturning? _totalMoveTime * 0.8f : _totalMoveTime * 0.4f;
	float lerpValue = (_capturingTime / halfTime);

	//FRotator CurrentRotator = GetActorRotation();
	//FRotator AddRotator = FRotator(180.f * DeltaTime, 360.f * DeltaTime, 0); // 각축 회전
	//FRotator NewRotator = CurrentRotator + AddRotator;
	//SetActorRotation(NewRotator);


	if (!_isReturning)
	{
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, lerpValue);
		SetActorLocation(NewLocation);

		if (_capturingTime >= halfTime)
		{
			// 반환 시작
			_capturingTime = 0.0f;
			_isReturning = true;
			TargetLocation = GetActorLocation();
		}
	}
	else
	{
		FVector playerLocation = _player->GetMesh()->GetSocketTransform(FName("tophead")).GetLocation();

		if (_isThrowing == true)
		{
			FVector NewLocation = FMath::Lerp(TargetLocation, playerLocation, lerpValue);
			SetActorLocation(NewLocation);
		}

		if (_capturingTime >= halfTime)
		{
			SetActorLocation(playerLocation);
			_isThrowing = false;
			_isReturning = false;
			_capturingTime = 0.0f;

		}
	}
}

void ACAS_Hat::Return()
{
	/*TargetLocation = GetActorLocation();

	_isThrowing = true;
	_isReturning = true;
	_capturingTime = 0.0f;*/
	_player->SetActorHiddenInGame(false);
	_player->SetActorEnableCollision(true);

	AttachToComponent(_player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시

	_testCaptureTarget = nullptr;

}

