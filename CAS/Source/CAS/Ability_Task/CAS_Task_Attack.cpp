// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Attack.h"
#include "Character/CAS_Character.h"

UCAS_Task_Attack* UCAS_Task_Attack::CAS_Task_Attack(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Attack* Task = NewAbilityTask<UCAS_Task_Attack>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Attack::Activate()
{
	if (!Montage) {
		return;
	}
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character->IsValidLowLevel()) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance|| AnimInstance->Montage_IsPlaying(Montage))
		{
			return;
		}
	
		AnimInstance->Montage_Play(Montage, PlayRate);
		
	}
}