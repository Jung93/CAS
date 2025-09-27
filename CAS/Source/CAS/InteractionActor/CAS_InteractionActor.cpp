// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionActor.h"
#include "UI/CAS_KeyPressUI.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACAS_InteractionActor::ACAS_InteractionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SenseCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	KeyPressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("KeyPressWidgetComponent"));
	RootComponent = StaticMesh;

	SenseCollider->SetupAttachment(StaticMesh);
	KeyPressWidgetComponent->SetupAttachment(StaticMesh);

	SenseCollider->SetCollisionProfileName(TEXT("InteractionSensor"));
	StaticMesh->SetCollisionProfileName(TEXT("InteractionActor"));

	static ConstructorHelpers::FClassFinder<UCAS_KeyPressUI> UIClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/CAS/Blueprint/UI/Interaction/BP_KeyPressUI.BP_KeyPressUI_C'"));

	if (UIClass.Succeeded())
	{
		KeyPressUIClass = UIClass.Class;
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureImage(TEXT("/Script/Engine.Texture2D'/Game/Assets/UI/Icon/T_e_256px_gray.T_e_256px_gray'"));

	if (TextureImage.Succeeded())
	{
		KeyTexture = TextureImage.Object;
	}
}

// Called when the game starts or when spawned
void ACAS_InteractionActor::BeginPlay()
{
	Super::BeginPlay();

	if (KeyPressUIClass) {
		KeyPressWidgetComponent->SetWidgetClass(KeyPressUIClass);
		KeyPressWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		KeyPressWidgetComponent->SetVisibility(true);

		KeyPressWidgetComponent->SetRelativeLocation(FVector::ZeroVector);
	}
	KeyPressUI = Cast<UCAS_KeyPressUI>(KeyPressWidgetComponent->GetUserWidgetObject());
	if (KeyTexture && KeyPressUI) {
		KeyPressUI->SetTexture(KeyTexture);
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	SenseCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	SenseCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlapEvent);
}

// Called every frame
void ACAS_InteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAS_InteractionActor::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (KeyPressUI) {
		KeyPressUI->SetVisibility(ESlateVisibility::Visible);
		bCanInteraction = true;	
	}
	
}

void ACAS_InteractionActor::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (KeyPressUI) {
		KeyPressUI->SetVisibility(ESlateVisibility::Collapsed);
		bCanInteraction = false;
	}
	
}