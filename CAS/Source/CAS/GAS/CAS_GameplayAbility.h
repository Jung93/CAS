// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_PlayerState.h"
#include "CAS_GamePlayTag.h"
#include "CAS_AttributeSet.h"
#include "CAS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
protected:
	virtual void ApplyGamePlayEffect(class ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, FGameplayEffectContextHandle EffectContext);
};
