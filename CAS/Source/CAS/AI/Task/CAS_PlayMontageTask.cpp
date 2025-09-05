// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_PlayMontageTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"


UCAS_PlayMontageTask::UCAS_PlayMontageTask()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UCAS_PlayMontageTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	auto CurPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Character = Cast<ACAS_Character>(CurPawn);
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (!Character) {
		return EBTNodeResult::Failed;
	}
	if (BlackBoard->GetValueAsBool(IsMontagePlayingKey.SelectedKeyName)) {
		return EBTNodeResult::Failed;
	}
	auto AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) {
		return EBTNodeResult::Failed;
	}
	
	if (!AnimInstance->Montage_IsPlaying(CAS_Montage)) {
		if (bPlayOnce) {
			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::MontageEnd);
		}
		else {
			bNotifyTick = true;
		}

		OwnerComp.GetAIOwner()->StopMovement();

		AnimInstance->Montage_Play(CAS_Montage);

		BlackBoard->SetValueAsBool(IsMontagePlayingKey.SelectedKeyName, true);

		CurrentTime = 0.0f;

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCAS_PlayMontageTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	CurrentTime += DeltaSeconds;

	if (CurrentTime >= LoopTime) {
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return;
	}
}

void UCAS_PlayMontageTask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	auto CurPawn = CachedOwnerComp->GetAIOwner()->GetPawn();
	auto CurCharacter = Cast<ACAS_Character>(CurPawn);
	auto AnimInstance = CurCharacter->GetMesh()->GetAnimInstance();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	BlackBoard->SetValueAsBool(IsMontagePlayingKey.SelectedKeyName, false);
	AnimInstance->Montage_Stop(0.2f, CAS_Montage);
}

void UCAS_PlayMontageTask::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	auto CurPawn = CachedOwnerComp->GetAIOwner()->GetPawn();
	auto CurCharacter = Cast<ACAS_Character>(CurPawn);
	auto AnimInstance = CurCharacter->GetMesh()->GetAnimInstance();
	
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::MontageEnd);
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	
	return;
}
