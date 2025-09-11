// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "CAS_GameplayCueNotifyStatic.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_GameplayCueNotifyStatic : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	

public:
	UCAS_GameplayCueNotifyStatic();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Param) const override;

	//UFUNCTION()
	//void SetAudioComponent(class UAudioComponent* Component) { AudioComp = Component; }

	//UFUNCTION()
	//class UAudioComponent* GetAudioComponent() { return AudioComp; }


	//UPROPERTY()
	//class UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<class USoundBase*> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<FGameplayTag> Tags;
};
