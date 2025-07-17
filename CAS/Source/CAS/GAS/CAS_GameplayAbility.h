// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_PlayerState.h"
#include "CAS_GamePlayTag.h"
#include "CAS_AttributeSet.h"
#include "GAS/CAS_AbilityInputID.h"
#include "CAS_GameplayAbility.generated.h"


USTRUCT(BlueprintType)
struct FCAS_SkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* AbilityIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityTag;

	UPROPERTY()
	EAbilityInputID InputID;
};

UCLASS()
class CAS_API UCAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCAS_GameplayAbility();

	const FCAS_SkillData& GetSkillData();

	void CAS_EndAbility();
public:
	UFUNCTION()
	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
	virtual void MontageEndEvent(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	FCAS_SkillData SkillData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* AbilityIcon;
	EAbilityInputID InputID = EAbilityInputID::None;
};
