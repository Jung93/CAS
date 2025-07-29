// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_PatrolTask.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAS_PatrolTask::UCAS_PatrolTask()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCAS_PatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());

	if (!Character || !Character->GetPatrolPath()) {
		return EBTNodeResult::Failed;
	}
	
	auto BlackBoard = AIController->GetBlackboardComponent();
	BlackBoard->SetValueAsVector("MovePosition", Character->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void UCAS_PatrolTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//플레이어 위치를 틱으로 체크
	//플레이어가 배열의 벡터에 근접하면 인덱스를 올림
	//패트롤패스[인덱스] 는 다음 무브포지션으로 블랙보드에 set
	//move to 로 이동
	auto AIController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto BlackBoard = AIController->GetBlackboardComponent();
	auto Character = Cast<ACAS_Character>(AIController->GetPawn());
	FVector CurrentPosition = Character->GetActorLocation();

	auto PatrolPath = Character->GetPatrolPath();
	int32 PatrolIndex = PatrolPath->GetPathIndex();
	FVector PatrolPosition = PatrolPath->GetPatrolPoint(PatrolIndex);
	int32 PatrolLength = PatrolPath->GetPatrolLength();

	if (FVector::DistSquared(CurrentPosition, PatrolPosition) < FMath::Square(10.0f)) {
		PatrolPath->IncreasePathIndex();
		auto NextPatrolPosition = PatrolPath->GetPatrolPoint(PatrolPath->GetPathIndex());
		BlackBoard->SetValueAsVector("MovePosition",NextPatrolPosition);
	}
	else {
		BlackBoard->SetValueAsVector("MovePosition", PatrolPosition);
	}

}
