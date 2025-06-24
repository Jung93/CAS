// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_ChargeAttack.h"

ACAS_EnemyCapt_ChargeAttack::ACAS_EnemyCapt_ChargeAttack()
{
}

void ACAS_EnemyCapt_ChargeAttack::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ACAS_EnemyCapt_ChargeAttack::AddDefaultAbilites()
{
	UCAS_AbilitySystemComponent* ASC = Cast<UCAS_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ASC) {
		return;
	}
	//DefaultAbilities.Add(EnemyAbility);
	ASC->AddCharacterAbilities(DefaultAbilities);
	FGameplayAbilitySpec Spec(EnemyAbility, 1, static_cast<int32>(EAbilityInputID::ChargeAttack));
	auto temp = ASC->GetActivatableAbilities();
	ASC->GiveAbility(Spec);
}
void ACAS_EnemyCapt_ChargeAttack::ActivateEnemyAbility()
{
	//ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.ChargeAttack"));
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::ChargeAttack));
}
void ACAS_EnemyCapt_ChargeAttack::ChargeReleased(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::ChargeAttack));
}
void ACAS_EnemyCapt_ChargeAttack::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Completed, this, &ThisClass::ChargeReleased);
	}

}

//void ACAS_EnemyCapt_ChargeAttack::ActivateAbility(const FInputActionValue& Value)
//{
//	//ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.ChargeAttack"));
//	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::ChargeAttack));
//}
