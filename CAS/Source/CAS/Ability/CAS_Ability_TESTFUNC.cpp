// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TESTFUNC.h"

UCAS_Ability_TESTFUNC::UCAS_Ability_TESTFUNC()
{
	AbilityTags.AddTag(CAS_GamePlayTag::Ability_Attack_TEST);
}

bool UCAS_Ability_TESTFUNC::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	return true;
}

void UCAS_Ability_TESTFUNC::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		UAnimInstance* AnimInstance = ActorInfo->AvatarActor->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
		if (AnimInstance && AttackMontage) // AttackMontage는 UAnimMontage* 변수로 미리 선언되어 있어야 합니다.
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
	if (ActorInfo && ActorInfo->OwnerActor.IsValid())
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		if (ASC)
		{
			auto Attribute = Cast<ACAS_Character>(ActorInfo->OwnerActor)->GetAttributeSet();
			float CurrHealth = Attribute->GetHealth();
			
			//dead 예외처리필요
			float HP = CurrHealth - 10.0f; // 체력감소
			
			Attribute->SetHealth(HP);
		}
	}
}

void UCAS_Ability_TESTFUNC::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
