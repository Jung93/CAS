// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAS_QuickSlotWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAS_API UCAS_QuickSlotWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCAS_QuickSlotWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitSetting(int32 count ){ PlayerAbilities.Init(nullptr, count); }
	
	void AddPlayerAbility(int32 index, const TSubclassOf<class UGameplayAbility>& newAbility);
	void RemovePlayerAbility();

	int32 FindEmptyPlayerAbilityIndex();

	FCAS_SlotData GetAbilityData(int32 index);
protected:

	UPROPERTY(VisibleAnywhere, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> PlayerAbilities;
};
