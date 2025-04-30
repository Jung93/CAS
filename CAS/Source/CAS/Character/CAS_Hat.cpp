// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_Hat.h"
#include "Components/SphereComponent.h"
#include "CAS/Character/CAS_EnemyCapt.h"

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
		_capturingTime += DeltaTime;

		float halfTime = _totalMoveTime / 2.0f;
		float lerpValue = _capturingTime / halfTime;

		if (!_isReturning)
		{
			FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, lerpValue);
			SetActorLocation(NewLocation);

			if (_capturingTime >= halfTime)
			{
				// 반환 시작
				_capturingTime = 0.0f;
				_isReturning = true;
			}
		}
		else
		{
			FVector NewLocation = FMath::Lerp(TargetLocation, StartLocation, lerpValue);
			SetActorLocation(NewLocation);

			if (_capturingTime >= halfTime)
			{
				_isThrowing = false;
				_isReturning = false;
				_capturingTime = 0.0f;
			}
		}

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

void ACAS_Hat::OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto enemy = Cast<ACAS_EnemyCapt>(OtherActor);

	if (!enemy->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("xcvmklwefrlqasxlmcdhlxmdcula"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy Detected!!"));

	_testCaptureTarget = enemy;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

}

void ACAS_Hat::ThrowHat(const FVector& direction)
{

	StartLocation = GetActorLocation();
	MoveDirection = direction.GetSafeNormal();
	TargetLocation = StartLocation + MoveDirection * 100.0f; // 예: 1000 거리까지

	_isThrowing = true;
	_capturingTime = 0.0f;
	_isReturning = false;

}

//void ACAS_Hat::ThrowHat()
//{
//	if (_isThrowing)
//		return;
//
//	_isThrowing = true;
//
//}

