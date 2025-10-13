// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_EnemyNoCapt.h"
#include "Components/CapsuleComponent.h"

ACAS_EnemyNoCapt::ACAS_EnemyNoCapt()
{
}

void ACAS_EnemyNoCapt::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CAS_EnemyNoCapt"));
}

void ACAS_EnemyNoCapt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAS_EnemyNoCapt::AddDefaultAbilites()
{
	UCAS_AbilitySystemComponent* ASC = Cast<UCAS_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ASC) {
		return;
	}
	ASC->AddCharacterAbilities(DefaultAbilities);
	FGameplayAbilitySpec Spec(EnemyAbility, 1, static_cast<int32>(EAbilityInputID::None));
	ASC->GiveAbility(Spec);

}

