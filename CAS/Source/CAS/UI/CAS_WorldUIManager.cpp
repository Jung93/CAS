// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_WorldUIManager.h"

// Sets default values for this component's properties
UCAS_WorldUIManager::UCAS_WorldUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WorldUIMananger_WIdgetComponent"));
	WidgetComponent->SetupAttachment(this);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	WidgetComponent->SetVisibility(false);
}


// Called when the game starts
void UCAS_WorldUIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCAS_WorldUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

