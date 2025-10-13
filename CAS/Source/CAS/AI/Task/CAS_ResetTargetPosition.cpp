// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ResetTargetPosition.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAS_BehaviorComponent.h"
#include "Navigation/PathFollowingComponent.h"

UCAS_ResetTargetPosition::UCAS_ResetTargetPosition()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCAS_ResetTargetPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();

	if (!Character || !PatrolPath) {
		return EBTNodeResult::Failed;
	}

	bool bInverse = BlackBoard->GetValueAsBool("bInversePatrolPath");
	int32 PatrolIndex = PatrolPath->GetPathIndex();

	if (!bInverse) {
		PatrolPath->IncreasePathIndex();
	}
	else {
		PatrolPath->DecreasePathIndex();
	}
	BlackBoard->SetValueAsInt(IndexKey.SelectedKeyName, PatrolPath->GetPathIndex());

	return EBTNodeResult::InProgress;
}

void UCAS_ResetTargetPosition::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	int32 PatrolIndex = BlackBoard->GetValueAsInt(IndexKey.SelectedKeyName);
	FVector CurrentPosition = Character->GetActorLocation();
	auto LocalPatrolPosition = PatrolPath->GetPatrolPoint(PatrolIndex);
	FVector PatrolPosition = PatrolPath->GetActorTransform().TransformPosition(LocalPatrolPosition);

	if (FVector::Distance(CurrentPosition, PatrolPosition) <= 50.0f) {
		
		BlackBoard->SetValueAsInt(IndexKey.SelectedKeyName, PatrolPath->GetPathIndex());
		auto NextLocalPatrolPosition = PatrolPath->GetPatrolPoint(PatrolPath->GetPathIndex());

		PatrolPosition = PatrolPath->GetActorTransform().TransformPosition(NextLocalPatrolPosition);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	}
	else {

		AIController->MoveToLocation(PatrolPosition, 50.0f, false);
	}
	

	return;
}
