// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_BgmManager.h"
#include "Global/CAS_GameInstance.h"

// Sets default values
ACAS_BgmManager::ACAS_BgmManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACAS_BgmManager::BeginPlay()
{
	Super::BeginPlay();

	UCAS_GameInstance* GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());

	if (GameInstance)
	{
		GameInstance->PlayBgm(BgmIndex);
	}
	
}

// Called every frame
void ACAS_BgmManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

