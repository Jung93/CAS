// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_LoadingOnlyActor.h"
#include "Blueprint/UserWidget.h"
#include "Global/CAS_GameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACAS_LoadingOnlyActor::ACAS_LoadingOnlyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACAS_LoadingOnlyActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (LoadingUIClass) {
		LoadingUI = CreateWidget<UUserWidget>(GetWorld(), LoadingUIClass);
		LoadingUI->AddToViewport(10);
		LoadingUI->SetVisibility(ESlateVisibility::Visible);
	}
	GetWorldTimerManager().SetTimer(Timer,this, &ThisClass::CheckReady,1.0f,true);
}

void ACAS_LoadingOnlyActor::CheckReady()
{
    auto GameInstance = Cast<UCAS_GameInstance>(GetGameInstance());

    if (GameInstance && GameInstance->IsSaveDataReady())
    {
		FName NextLevelName = GameInstance->GetNextLevelName();
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
    }
}
// Called every frame
void ACAS_LoadingOnlyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

