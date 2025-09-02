// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_DetectTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"

EBTNodeResult::Type UCAS_DetectTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	auto CurPawn = CachedOwnerComp->GetAIOwner()->GetPawn();
	auto CurCharacter = Cast<ACAS_Character>(CurPawn);

	if (!CurCharacter) {
		return EBTNodeResult::Failed;
	}

	auto AnimInstance = CurCharacter->GetMesh()->GetAnimInstance();

	if (!AnimInstance) {
		return EBTNodeResult::Failed;
	}
	
	if (!AnimInstance->Montage_IsPlaying(DetectMontage))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::MontageEnd);
		AnimInstance->Montage_Play(DetectMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	}
	return EBTNodeResult::InProgress;
}

void UCAS_DetectTask::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	auto CurPawn = CachedOwnerComp->GetAIOwner()->GetPawn();
	auto CurCharacter = Cast<ACAS_Character>(CurPawn);
	auto AnimInstance = CurCharacter->GetMesh()->GetAnimInstance();

	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::MontageEnd);
	FinishLatentTask(*CachedOwnerComp,EBTNodeResult::Succeeded);
}


