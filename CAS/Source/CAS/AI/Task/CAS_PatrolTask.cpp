// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_PatrolTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAS_BehaviorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"        
#include "NavigationPath.h"  

UCAS_PatrolTask::UCAS_PatrolTask()
{
	bNotifyTick = true;
	
}

EBTNodeResult::Type UCAS_PatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();

	if (!Character || !PatrolPath) {
		return EBTNodeResult::Failed;
	}
	int32 NextIndex = BlackBoard->GetValueAsInt(IndexKey.SelectedKeyName);
	FVector LocalPatrolPosition = PatrolPath->GetPatrolPoint(NextIndex);
	FVector PatrolPosition = PatrolPath->GetActorTransform().TransformPosition(LocalPatrolPosition);
	
	
	BlackBoard->SetValueAsVector(MoveVectorKey.SelectedKeyName, PatrolPosition);

	FAIRequestID MoveID = AIController->MoveToLocation(PatrolPosition, 50.0f, false);

	return EBTNodeResult::InProgress;
}

void UCAS_PatrolTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto BehaviorTypeComponent = AIController->GetBehaviorComponent();
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	auto PatrolPath = Character->GetPatrolPath();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	FVector CurrentPosition = Character->GetActorLocation();
	FVector PatrolPosition = BlackBoard->GetValueAsVector(MoveVectorKey.SelectedKeyName);
	bool bInverse = BlackBoard->GetValueAsBool("bInversePatrolPath");

	FNavLocation NavLocation;
	auto NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	UNavigationPath* Path = NavSystem->FindPathToLocationSynchronously(GetWorld(), Character->GetActorLocation(), PatrolPosition);
	if (!NavSystem || !Path || Path->IsPartial()) {

		bInverse = !bInverse;
		BlackBoard->SetValueAsBool("bInversePatrolPath", bInverse);
		if (!bInverse) {
			PatrolPath->IncreasePathIndex();
		}
		else {
			PatrolPath->DecreasePathIndex();
		}

		BlackBoard->SetValueAsInt(IndexKey.SelectedKeyName, PatrolPath->GetPathIndex());
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (FVector::Distance(CurrentPosition, PatrolPosition) <= 50.0f) {

		if (!bInverse) {
			PatrolPath->IncreasePathIndex();
		}
		else {
			PatrolPath->DecreasePathIndex();
		}
		BlackBoard->SetValueAsInt(IndexKey.SelectedKeyName, PatrolPath->GetPathIndex());

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	}

	return;
}
