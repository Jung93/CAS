// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_PlayMontage.h"

UCAS_Task_PlayMontage* UCAS_Task_PlayMontage::Task_PlayMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_PlayMontage* Task = NewAbilityTask<UCAS_Task_PlayMontage>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_PlayMontage::Activate()
{
	Super::Activate();

	if (!Montage) {
		return;
	}
	
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance)
		{
			return;
		}
		
		UCAS_GameplayAbility* ability = Cast<UCAS_GameplayAbility>(Ability);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(ability, &UCAS_GameplayAbility::PlayAnimNotify);
		AnimInstance->Montage_Play(Montage, PlayRate);
	}
}

void UCAS_Task_PlayMontage::OnDestroy(bool bInOwnerFinished)
{
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		UCAS_GameplayAbility* ability = Cast<UCAS_GameplayAbility>(Ability);
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(ability, &UCAS_GameplayAbility::PlayAnimNotify);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}
