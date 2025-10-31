// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_PlayMontage.h"


UCAS_Task_PlayMontage* UCAS_Task_PlayMontage::Task_PlayMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate,  bool bNotifyReady, FName SectionName)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_PlayMontage* Task = NewAbilityTask<UCAS_Task_PlayMontage>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;
	Task->bNotifyReady = bNotifyReady;
	Task->MontageSection = SectionName;
	return Task;
}

void UCAS_Task_PlayMontage::JumpToSection(FName SectionName)
{
	if (!AnimInstance || !AnimInstance->Montage_IsPlaying(Montage))
		return;

	AnimInstance->Montage_JumpToSection(SectionName);

}

void UCAS_Task_PlayMontage::Activate()
{
	Super::Activate();

	if (!Montage) {
		return;
	}
	
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character) {

		AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance)
		{
			return;
		}
		
		UCAS_GameplayAbility* ability = Cast<UCAS_GameplayAbility>(Ability);

		if(bNotifyReady){
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(ability, &UCAS_GameplayAbility::PlayAnimNotify);
		}
		AnimInstance->OnMontageEnded.AddDynamic(ability,&UCAS_GameplayAbility::MontageEndEvent);
		AnimInstance->Montage_Play(Montage, PlayRate);
		AnimInstance->Montage_JumpToSection(MontageSection);
	}



}

void UCAS_Task_PlayMontage::OnDestroy(bool bInOwnerFinished)
{
	auto Character = Cast<ACAS_Character>(GetAvatarActor());

	if (!Character || !Ability) {
		Super::OnDestroy(bInOwnerFinished);
		return;
	}
	UCAS_GameplayAbility* ability = Cast<UCAS_GameplayAbility>(Ability);
	if (bNotifyReady) {
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(ability, &UCAS_GameplayAbility::PlayAnimNotify);
	}
	AnimInstance->OnMontageEnded.RemoveDynamic(ability, &UCAS_GameplayAbility::MontageEndEvent);

	Super::OnDestroy(bInOwnerFinished);
}
