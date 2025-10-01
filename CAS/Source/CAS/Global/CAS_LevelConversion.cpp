// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_LevelConversion.h"
#include "Character/CAS_Player.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Global/CAS_GameInstance.h"

// Sets default values
ACAS_LevelConversion::ACAS_LevelConversion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	volume->SetCollisionProfileName(TEXT("Trigger"));
	volume->SetHiddenInGame(false);
	RootComponent = volume;
}

// Called when the game starts or when spawned
void ACAS_LevelConversion::BeginPlay()
{
	Super::BeginPlay();

	if (NextLevel.IsValid()) {
		NextLevelName = FName(*NextLevel.GetAssetName());
	}
	volume->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnOverlapBegin);
}

void ACAS_LevelConversion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<ACAS_Player>(OtherActor);
	if (!Player) {
		return;
	}

	auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());
	GameInstance->SetNextLevelName(NextLevelName.ToString());
	Player->SaveCharacterData();

	GameInstance->OpenLoadingLevel();

}
