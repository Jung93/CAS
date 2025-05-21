// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Dead.h"

UCAS_Task_Dead* UCAS_Task_Dead::Task_Dead(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Dead* Task = NewAbilityTask<UCAS_Task_Dead>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Dead::Activate()
{
	Super::Activate();

	if (!Montage) {
		return;
	}
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character->IsValidLowLevel()) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance)
		{
			return;
		}

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::PlayAnimNotify);
		AnimInstance->Montage_Play(Montage, PlayRate);
	}
}

void UCAS_Task_Dead::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}
/*
Todo
체력이 0되면 델리게이트로 플레이어의 데드어빌리티 실행
데드어빌리티 -> 즉시 데드 태그를 달고
->태스크실행 ->애님노티파이로 사라지게처리->어빌리티 종료

데드태그 -> 행동트리에서 막아야함 , 데드태그를 달면 하던공격취소 및 데미지 x


*/