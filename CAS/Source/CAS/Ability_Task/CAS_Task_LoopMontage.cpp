// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_LoopMontage.h"

UCAS_Task_LoopMontage* UCAS_Task_LoopMontage::Task_LoopMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_LoopMontage* Task = NewAbilityTask<UCAS_Task_LoopMontage>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;
	return Task;
}

void UCAS_Task_LoopMontage::Activate()
{
	Super::Activate();

	if (!Ability || !Montage)
	{
		EndTask();
		return;
	}

	AActor* Avatar = GetAvatarActor();
	if (!Avatar)
	{
		EndTask();
		return;
	}

	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		EndTask();
		return;
	}

	AnimInstance->Montage_Play(Montage, PlayRate);

	GetWorld()->GetTimerManager().SetTimer(MontageLoopTimerHandle, this, &UCAS_Task_LoopMontage::CheckMontageLoop, 0.05f, true);
}

void UCAS_Task_LoopMontage::OnDestroy(bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearTimer(MontageLoopTimerHandle);
	Super::OnDestroy(bInOwnerFinished);
}

void UCAS_Task_LoopMontage::CheckMontageLoop()
{
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		EndTask();
		return;
	}
	if (!AnimInstance->Montage_IsPlaying(Montage)) {

		AnimInstance->Montage_Play(Montage, PlayRate);
	}
}
