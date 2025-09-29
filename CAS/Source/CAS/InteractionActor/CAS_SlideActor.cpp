#include "InteractionActor/CAS_SlideActor.h"
#include "Character/CAS_Player.h"
#include "Components/WidgetComponent.h"

void ACAS_SlideActor::InteractionWithPlayer()
{
	auto Controller = GetWorld()->GetFirstPlayerController();
	if (!Controller) {
		return;
	}
	auto Player = Cast<ACAS_Player>(Controller->GetPawn());

	if (Player->IsInteracting) {
		Player->bPositionReceived = true;
		
		FVector Position = GetActorTransform().TransformPosition(ActorPosition);
		Player->ReceivedPosition = Position;
	}
}

void ACAS_SlideActor::BeginPlay()
{
	Super::BeginPlay();

	KeyPressWidgetComponent->SetRelativeLocation(ActorPosition + FVector(0, 0, -5.0f));
}
