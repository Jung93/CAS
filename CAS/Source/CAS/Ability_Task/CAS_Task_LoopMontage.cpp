// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_LoopMontage.h"

UCAS_Task_LoopMontage* UCAS_Task_LoopMontage::Task_LoopMontage(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, FName StartSection, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_LoopMontage* Task = NewAbilityTask<UCAS_Task_LoopMontage>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;
	Task->SectionName = StartSection;
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

	PlayMontage();

	// 타이머 반복 설정: 재생 상태 감시
	GetWorld()->GetTimerManager().SetTimer(MontageLoopTimerHandle, this, &UCAS_Task_LoopMontage::CheckMontageLoop, 0.05f, true);
}

void UCAS_Task_LoopMontage::OnDestroy(bool bInOwnerFinished)
{
}

void UCAS_Task_LoopMontage::PlayMontage()
{
}

void UCAS_Task_LoopMontage::CheckMontageLoop()
{
}

void UCAS_Task_LoopMontage::StopMontage()
{
}
