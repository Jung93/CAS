// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/CAS_SkillSlot.h"
#include "CAS_SelectSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SelectSkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSetting();

	void SetSlots(const TArray<UCAS_SkillSlot*> CurrentSkillSlots, const TSubclassOf<class UGameplayAbility>& newAbility);
	void SetSlots(int32 TargetIndex, TArray<UCAS_SkillSlot*> TargetSkillSlots);

	TArray<UCAS_SkillSlot*>& GetSkillSlots() { return SkillSlots; }


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UCAS_SkillSlot> SlotWidgetClass;
	UPROPERTY()
	TArray<UCAS_SkillSlot*> SkillSlots;

	UPROPERTY()
	TSubclassOf<class UGameplayAbility> TargetAbility;
};
