// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ScanLostTargetTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "AI/CAS_BehaviorComponent.h"
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

	AnimInstance->Montage_Play(Montage);

	FScanTimeMemory* memory = (FScanTimeMemory*)NodeMemory;
	memory->CurTime = 0.0f;

	return EBTNodeResult::InProgress;
}

void UCAS_ScanLostTargetTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
		
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	EBehaviorType curType = static_cast<EBehaviorType>(BlackBoard->GetValueAsEnum(BehaviorTypeKey.SelectedKeyName));
	
	auto AIController = OwnerComp.GetAIOwner();
	AIController->StopMovement();

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
				
		BlackBoard->SetValueAsBool(IsMontagePlayingKey.SelectedKeyName, false);

		AnimInstance->Montage_Stop(0.25f, Montage);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
