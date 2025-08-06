// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ScanLostTargetTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"


UCAS_ScanLostTargetTask::UCAS_ScanLostTargetTask()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCAS_ScanLostTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* CurPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Character = Cast<ACAS_Character>(CurPawn);
	if (!Character) {
		return EBTNodeResult::Failed;
	}

	auto AnimInstance = Character->GetMesh()->GetAnimInstance();

	if (!AnimInstance) {
		return EBTNodeResult::Failed;
	}
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (BlackBoard->GetValueAsBool("bPlayerLost") == false)
	{
		return EBTNodeResult::Failed;
	}

	FScanTimeMemory* memory = (FScanTimeMemory*)NodeMemory;
	memory->CurTime = 0.0f;

	AnimInstance->Montage_Play(Montage);

	return EBTNodeResult::InProgress;
}

void UCAS_ScanLostTargetTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
		
	FScanTimeMemory* memory = (FScanTimeMemory*)NodeMemory;
	memory->CurTime += DeltaSeconds;


	if (memory->CurTime >= PlayTime) {
		
		APawn* CurPawn = OwnerComp.GetAIOwner()->GetPawn();

		auto Character = Cast<ACAS_Character>(CurPawn);
		if (!Character) {
			return;
		}

		auto AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance) {
			return;
		}
				
		AnimInstance->Montage_Stop(0, Montage);


		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
