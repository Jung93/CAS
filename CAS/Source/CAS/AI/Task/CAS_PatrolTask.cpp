// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_PatrolTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAS_PatrolTask::UCAS_PatrolTask()
{

}

EBTNodeResult::Type UCAS_PatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (!Character || !PatrolPath) {
		return EBTNodeResult::Failed;
	}


	FVector CurrentPosition = Character->GetActorLocation();
	
	int32 PatrolIndex = PatrolPath->GetPathIndex();
	auto LocalPatrolPosition = PatrolPath->GetPatrolPoint(PatrolIndex);

	FVector PatrolPosition = PatrolPath->GetActorTransform().TransformPosition(LocalPatrolPosition);

	auto temp = FVector::Distance(CurrentPosition, PatrolPosition);
	if (FVector::Distance(CurrentPosition, PatrolPosition) <= 100.0f) {
		PatrolPath->IncreasePathIndex();
		auto NextLocalPatrolPosition = PatrolPath->GetPatrolPoint(PatrolPath->GetPathIndex());
		
		PatrolPosition = PatrolPath->GetActorTransform().TransformPosition(NextLocalPatrolPosition);
		BlackBoard->SetValueAsVector(MovePositionKey.SelectedKeyName, PatrolPosition);

	}
	else {
		
		BlackBoard->SetValueAsVector(MovePositionKey.SelectedKeyName, PatrolPosition);
	}

	return EBTNodeResult::Succeeded;
}

