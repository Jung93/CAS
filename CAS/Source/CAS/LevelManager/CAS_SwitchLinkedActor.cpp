// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SwitchLinkedActor.h"
#include "InteractionActor/CAS_InteractionSwitch.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ACAS_SwitchLinkedActor::ACAS_SwitchLinkedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACAS_SwitchLinkedActor::BeginPlay()
{
	Super::BeginPlay();

	// 스위치 하나가 모든 linkedActors를 조작 -> 각각의 linkedActor가 조작해줄 스위치를 정해서
	if (SwitchInstance) {
		SwitchInstance->OnSwitchClicked.AddUObject(this, &ThisClass::SwitchClicked);
	}
}

// Called every frame
void ACAS_SwitchLinkedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

