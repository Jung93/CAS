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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SenseCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	NPC_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	KeyPressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("KeyPressWidgetComponent"));
	RootComponent = NPC_Mesh;

	SenseCollider->SetupAttachment(NPC_Mesh);
	KeyPressWidgetComponent->SetupAttachment(NPC_Mesh);
	SenseCollider->SetCollisionProfileName(TEXT("NPC"));
}

// Called when the game starts or when spawned
void ACAS_SaveNPC::BeginPlay()
{
	Super::BeginPlay();

	if (KeyPressUIClass) {
		KeyPressWidgetComponent->SetWidgetClass(KeyPressUIClass);
		KeyPressWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		KeyPressWidgetComponent->SetVisibility(true);

		KeyPressWidgetComponent->SetRelativeLocation(FVector(0, 0, 88.0f));
	}
	KeyPressUI = Cast<UCAS_KeyPressUI>(KeyPressWidgetComponent->GetUserWidgetObject());
	if (KeyTexture && KeyPressUI) {
		KeyPressUI->SetTexture(KeyTexture);
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	SenseCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	SenseCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlapEvent);

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

void ACAS_SaveNPC::InteractionWithPlayer()												
{
	SaveLoadWidget->DisplaySaveLoadWidget();
}

void ACAS_SaveNPC::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (KeyPressUI) {
		KeyPressUI->SetVisibility(ESlateVisibility::Visible);
		bCanInteraction = true;
	}
}

void ACAS_SaveNPC::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (KeyPressUI) {
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
		bCanInteraction = false;
	}
	SaveLoadWidget->CloseSaveLoadWidget();
}
