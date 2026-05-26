// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_Hat.h"
#include "Components/SphereComponent.h"
#include "CAS/Character/CAS_EnemyCapt.h"
#include "CAS/Character/CAS_Player.h"
#include "CAS/Character/CAS_HitScan.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

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
	AttachToComponent(_player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("index_02_r"));
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

		//기절 상태일 경우 빙의 안함
		if (asc->HasMatchingGameplayTag(tag))
			return;

		//빙의 대상에게 빙의 처리
		_testCaptureTarget = enemy;
		_testCaptureTarget->BeCaptured(this);

		_isThrowing = false;
		_isReturning = false;

		//빙의 대상의 메쉬에 hat 부착
		AttachToComponent(_testCaptureTarget->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시

		//플레이어 숨김 처리
		_player->SetActorHiddenInGame(true);
		_player->SetActorEnableCollision(false);
		_player->GetHitScan()->SetActorEnableCollision(false);

		return;
	}

	if (_isReturning)
	{
		//빙의없이 다시 돌아올 때 플레이어의 메쉬에 hat 부착
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

	//빙의 어빌리티 사용 시 hat 이 날아갈 위치와 방향
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
	//hat 의 전진, 복귀

	_capturingTime += DeltaTime;

	float halfTime = _isReturning? _totalMoveTime * 0.8f : _totalMoveTime * 0.4f;
	float lerpValue = (_capturingTime / halfTime);

	if (!_isReturning)
	{
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, lerpValue);
		SetActorLocation(NewLocation);

		if (_capturingTime >= halfTime)
		{
			//복귀 시작
			_capturingTime = 0.0f;
			_isReturning = true;
			TargetLocation = GetActorLocation();
		}
	}
	else
	{
		FVector playerLocation = _player->GetMesh()->GetSocketTransform(FName("tophead")).GetLocation();

		//플레이어 위치를 계산해서 복귀
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

	//빙의 해제 시 플레이어 숨김 처리 해제
	_player->SetActorHiddenInGame(false);
	_player->SetActorEnableCollision(true);
	_player->GetHitScan()->SetActorEnableCollision(true);

	AttachToComponent(_player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("tophead")); // 소켓 이름 "head" 예시

	_testCaptureTarget = nullptr;
	_isThrowing = false;
	_isReturning = false;
}

