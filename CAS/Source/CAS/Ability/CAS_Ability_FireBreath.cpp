// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_FireBreath.h"
#include "Ability_Task/CAS_Task_FireBreath.h"
#include "Controller/CAS_PlayerController.h"

UCAS_Ability_FireBreath::UCAS_Ability_FireBreath()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_FireBreath::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_FireBreath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_FireBreath::Task_FireBreath(this, "FireBreath");

	if (Task->IsValidLowLevel())
	{
		Task->OnAttackHit.AddUObject(this, &ThisClass::ReceiveTarget);
		Task->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		ReceiveTarget(owner, 1);

	}

}

void UCAS_Ability_FireBreath::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

FActiveGameplayEffectHandle UCAS_Ability_FireBreath::ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	FGameplayEffectSpecHandle TagSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TagEffectClass, 1.0f, EffectContext);

	if (SpecHandle.IsValid() && TagSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*TagSpecHandle.Data);

	
		TargetAbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.FireBreath"));

		return FActiveGameplayEffectHandle();
	}

	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle UCAS_Ability_FireBreath::ApplyGamePlayEffectToTarget(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.FireBreath")), -1.0f);
		FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);

		FGameplayCueParameters params;
		params.Location = TargetAbilitySystemComp->GetOwnerActor()->GetActorLocation();

		TargetAbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.OnFire"));

		return Handle;
	}

	return FActiveGameplayEffectHandle();
}

void UCAS_Ability_FireBreath::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
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

	auto owner = Cast<ACAS_Character>(GetAvatarActorFromActorInfo());

	if (owner == Target)
	{
		ApplyGamePlayEffectToSelf(Target, DamageEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
	}
	else
	{
		ApplyGamePlayEffectToTarget(Target, DamageEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
	}
}
