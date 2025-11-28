// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor/CAS_InteractionActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "UI/CAS_KeyPressMovementUI.h"
#include "CAS_InteractionMirror.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_InteractionMirror : public ACAS_InteractionActor
{
	GENERATED_BODY()
public:
	ACAS_InteractionMirror();

	virtual void InteractionWithPlayer()override;

	void SetLaserActivated(bool LaserActivated);
	bool GetLaserActivated();
	void ResetMirrorTransform();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void WidgetClicked(EWidgetPositionType Type);

	virtual void OnOverlapEvent(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult) override;
	virtual void EndOverlapEvent(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void OpenMovementUIMode();
	void ExitMovementUIMode();
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCAS_KeyPressMovementUI> MovementUIClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* MovementUIWidgetComponent;
	UPROPERTY()
	class UCAS_KeyPressMovementUI* MovementUI = nullptr;
	UPROPERTY(EditAnywhere, Category = "UI")
	TArray<FMovementUIInfo> UIInfo;
protected:
	UPROPERTY(EditAnywhere)
	class UCAS_LaserComponent* LaserComponent;

	FTransform InitTransform;

	UPROPERTY(EditAnywhere)
	float Degree = 5.0f;

	UPROPERTY(EditAnywhere,meta = (MakeEditWidget = "true"))
	FVector TargetLocation = FVector::ZeroVector;
};
