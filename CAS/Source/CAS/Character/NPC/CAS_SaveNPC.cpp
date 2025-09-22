// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/CAS_SaveNPC.h"
#include "UI/CAS_KeyPressUI.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "UI/CAS_SaveLoadWidget.h"

// Sets default values
ACAS_SaveNPC::ACAS_SaveNPC()
{

}

// Called when the game starts or when spawned
void ACAS_SaveNPC::BeginPlay()
{
	Super::BeginPlay();

	SaveLoadWidget = CreateWidget<UCAS_SaveLoadWidget>(GetWorld(), SaveLoadWidgetClass);
	SaveLoadWidget->bSaveMode = true;
	SaveLoadWidget->AddToViewport(3);
	SaveLoadWidget->CloseSaveLoadWidget();

}

// Called every frame
void ACAS_SaveNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAS_SaveNPC::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ACAS_SaveNPC::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlapEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	SaveLoadWidget->CloseSaveLoadWidget();
}

void ACAS_SaveNPC::InteractionWithPlayer()												
{
	SaveLoadWidget->DisplaySaveLoadWidget();
}

