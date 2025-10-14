// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_DestroyVolumeActor.h"
#include "Components/BoxComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"

// Sets default values
ACAS_DestroyVolumeActor::ACAS_DestroyVolumeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	DestroyVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	DestroyVolume->SetCollisionProfileName(TEXT("Trigger"));
	DestroyVolume->SetHiddenInGame(true);
	RootComponent = DestroyVolume;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

// Called when the game starts or when spawned
void ACAS_DestroyVolumeActor::BeginPlay()
{
	Super::BeginPlay();
	DestroyVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}
void ACAS_DestroyVolumeActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Character = Cast<ACAS_Character>(OtherActor)) {
		Character->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Dead"));
		return;
	}
	else {
		DestroyThisActor(OtherActor);
	}
}
void ACAS_DestroyVolumeActor::DestroyThisActor(AActor* Actor)
{
	Actor->Destroy();
}

// Called every frame
void ACAS_DestroyVolumeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

