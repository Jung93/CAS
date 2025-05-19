// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_AttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

UCAS_AttributeSet::UCAS_AttributeSet()
{
	InitHealth(3.0f);
	InitMaxHealth(6.0f);
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
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		int32 Hp = static_cast<int32>(GetHealth());
		if (Hp <= 0)
		{
			FString DebugMessage = FString::Printf(TEXT("Current Health: %.2f"), GetHealth());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage);
		}
		else {
			HpChanged.Broadcast(Hp);
		}
	}
}
