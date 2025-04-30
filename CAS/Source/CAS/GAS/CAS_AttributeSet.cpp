// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_AttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

UCAS_AttributeSet::UCAS_AttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.f);
}

void UCAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UCAS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
