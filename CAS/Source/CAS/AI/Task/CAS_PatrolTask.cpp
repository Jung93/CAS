// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_PatrolTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAS_BehaviorComponent.h"

UCAS_PatrolTask::UCAS_PatrolTask()
{

}

EBTNodeResult::Type UCAS_PatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();
	
	if (!Character || !PatrolPath) {
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UCAS_PatrolTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto BehaviorTypeComponent = AIController->GetBehaviorComponent();

	if (!BehaviorTypeComponent->IsBehaviorType(EBehaviorType::Patrol)) {
		//auto CurrType = BehaviorTypeComponent->GetBehaviorType();
		//BehaviorTypeComponent->ChangeBehaviorType(CurrType);

		FinishLatentAbort(OwnerComp);
		return;
	}
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

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
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return;
}

