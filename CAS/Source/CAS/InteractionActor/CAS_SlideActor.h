#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "CAS_SlideActor.generated.h"

/**
 *
 */
UCLASS()
class CAS_API ACAS_SlideActor : public ACAS_InteractionActor
{
	GENERATED_BODY()

public:
	virtual void InteractionWithPlayer() override;

	UPROPERTY(EditAnywhere, Category = "ActorPosition", meta = (MakeEditWidget = "true"))
	FVector ActorPosition = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;
};