// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_PortalActor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Character/CAS_Player.h"
#include "Global/CAS_GameInstance.h"
#include "LevelManager/CAS_BallTargetPoint.h"
// Sets default values
ACAS_PortalActor::ACAS_PortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PortalCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	PortalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NaiagaraEffect"));

	SetRootComponent(RootScene);

	StaticMesh->SetupAttachment(RootComponent);
	PortalCollider->SetupAttachment(RootComponent);
	PortalEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACAS_PortalActor::BeginPlay()
{
	Super::BeginPlay();

	if (NextLevel) {
		NextLevelName = FName(*NextLevel->GetName());
	}

	PortalCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);

	////목표에 공이 들어가면 델리게이트로 모든 목표가 true인지 확인하는 함수 바인딩
	//for (auto BallTargetPoint : BallTargetPoints) {
	//	
	//	//BallTargetPoint
	//}
}
void ACAS_PortalActor::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Player = Cast<ACAS_Player>(OtherActor)) {
		
		auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());
		GameInstance->SetNextLevelName(NextLevelName.ToString());
		Player->SaveCharacterData();

		GameInstance->OpenLoadingLevel();
	}
	else {
		return;
	}
}

