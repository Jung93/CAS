// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_SetFocusTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "Kismet/KismetMathLibrary.h"

UCAS_SetFocusTask::UCAS_SetFocusTask()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCAS_SetFocusTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto TargetActor = BlackBoard->GetValueAsObject(TargetActorKey.SelectedKeyName);
	
	if (!TargetActor) {
		return EBTNodeResult::Failed;
	}
	auto TargetCharacter = Cast<ACAS_Character>(TargetActor);

	if (!TargetCharacter) {
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->SetFocus(TargetCharacter);
	return EBTNodeResult::InProgress;
}

void UCAS_SetFocusTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) { 
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);;
		return;
	}
	auto ThisPawn = AIController->GetPawn();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto TargetActor = BlackBoard->GetValueAsObject(TargetActorKey.SelectedKeyName);

	if (!ThisPawn||!TargetActor) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);;
		return;
	}
	auto TargetCharacter = Cast<ACAS_Character>(TargetActor);

	if (!TargetCharacter) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);;
		return;
	}
	
	FVector Direction = TargetCharacter->GetActorLocation() - ThisPawn->GetActorLocation();
	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = AIController->GetControlRotation();
	FRotator NextRotation = FMath::RInterpConstantTo(CurrentRotation, Direction.Rotation(), DeltaSeconds, 20.0f);
	AIController->SetControlRotation(NextRotation);

	float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(NextRotation.Yaw, TargetRotation.Yaw));
	if (YawDiff <= 10.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
