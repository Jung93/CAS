// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_StunTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAS_StunTask::UCAS_StunTask()
{
	NodeName = TEXT("StunTask");
	bNotifyTick = true;
}

EBTNodeResult::Type UCAS_StunTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FStunTimeMemory* memory = (FStunTimeMemory*)NodeMemory;
	memory->CurTime = 0.0f;

	AnimInstance->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	
	return EBTNodeResult::InProgress;
}

void UCAS_StunTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* CurPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Character = Cast<ACAS_Character>(CurPawn);
	if (!Character) {
		return;
	}
	auto AIController = OwnerComp.GetAIOwner();
	AIController->StopMovement();

	FStunTimeMemory* memory = (FStunTimeMemory*)NodeMemory;
	memory->CurTime += DeltaSeconds;

	if (memory->CurTime >= PlayTime) {
		
		auto AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance) {
			return;
		}
		auto BlackBoard = OwnerComp.GetBlackboardComponent();
		BlackBoard->SetValueAsBool(IsMontagePlayingKey.SelectedKeyName, false);

		float blendtime = AnimInstance->Montage_GetBlendTime(Montage);
		AnimInstance->Montage_Stop(blendtime, Montage);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return;
	}
}
