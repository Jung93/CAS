// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/CAS_SaveNPC.h"
#include "UI/CAS_KeyPressUI.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACAS_SaveNPC::ACAS_SaveNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SenseCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	NPC_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	KeyPressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("KeyPressWidgetComponent"));
	RootComponent = NPC_Mesh;

	SenseCollider->SetupAttachment(NPC_Mesh);
	SenseCollider->SetCollisionProfileName(TEXT("SaveNPC"));
}

// Called when the game starts or when spawned
void ACAS_SaveNPC::BeginPlay()
{
	Super::BeginPlay();

	if (KeyPressUIClass) {
		KeyPressWidgetComponent->SetWidgetClass(KeyPressUIClass);
	}
	auto KeyPressWidget = Cast<UCAS_KeyPressUI>(KeyPressWidgetComponent->GetWidget());
	if (KeyTexture || KeyPressWidget) {
		KeyPressWidget->SetTexture(KeyTexture);
		KeyPressWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	SenseCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	SenseCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlapEvent);
}

// Called every frame
void ACAS_SaveNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	if (playerCameraManager)
	{
		FVector widgetLocation = KeyPressWidgetComponent->GetComponentLocation();
		FVector cameraLocation = playerCameraManager->GetCameraLocation();
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(widgetLocation, cameraLocation);
		KeyPressWidgetComponent->SetWorldRotation(rotation);
	}
}

void ACAS_SaveNPC::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	KeyPressWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
}

void ACAS_SaveNPC::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	KeyPressWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
}
