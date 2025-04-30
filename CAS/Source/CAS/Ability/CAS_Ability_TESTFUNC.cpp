// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TESTFUNC.h"

UCAS_Ability_TESTFUNC::UCAS_Ability_TESTFUNC()
{
	//AbilityTags.AddTag(CAS_GamePlayTag::Ability_Attack_TEST); 코드로 작성해도 되지만 블루프린트에서 하는게 더 디자이너 친화적
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

	if (!AttackMontage) {
		return;
	}
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		UAnimInstance* AnimInstance = ActorInfo->AvatarActor->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
		if (AnimInstance){
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
	if (ActorInfo && ActorInfo->OwnerActor.IsValid())
	{
		auto Character = Cast<ACAS_Character>(ActorInfo->AvatarActor);

		if (Character)
		{
			auto Attribute = Character->GetAttributeSet();
			float CurrHealth = Attribute->GetHealth();
		
			float HP = CurrHealth - 10.0f; 
			
			Attribute->SetHealth(HP);
		}
	}
}

void UCAS_Ability_TESTFUNC::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
