// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TakeDamage.h"

UCAS_Ability_TakeDamage::UCAS_Ability_TakeDamage()
{
}

bool UCAS_Ability_TakeDamage::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_TakeDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", TakeDamageMontage, 0.5f, false);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}

	auto owner = Cast<ACAS_Character>(GetAvatarActorFromActorInfo());

	ReceiveTarget(owner, 1);

}

void UCAS_Ability_TakeDamage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UCAS_Ability_TakeDamage::MontageEndEvent(UAnimMontage* Montage, bool bInterrupted)
{
	CAS_EndAbility();
}

void UCAS_Ability_TakeDamage::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST")), -1.0f);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
	}
}

void UCAS_Ability_TakeDamage::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = nullptr;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();

	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
		}
	}
	if (!AbilitySystemComp->IsValidLowLevel()) {
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
	EffectContextHandle.AddInstigator(PlayerState, nullptr);

	ApplyGamePlayEffect(Target, DamageEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
	ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);



}
