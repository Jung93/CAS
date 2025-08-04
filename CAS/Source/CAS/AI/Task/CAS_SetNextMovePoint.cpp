// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_SetNextMovePoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"

UCAS_SetNextMovePoint::UCAS_SetNextMovePoint()
{
}

EBTNodeResult::Type UCAS_SetNextMovePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto Target = BlackBoard->GetValueAsObject(TargetKey.SelectedKeyName);
	auto Player = Cast<ACAS_Character>(Target);

	if (!Target||!Player) {
		return EBTNodeResult::Failed;
	}

	FVector PlayerPosition = Player->GetActorLocation();

	BlackBoard->SetValueAsVector(MovePositionKey.SelectedKeyName, PlayerPosition);

	return EBTNodeResult::Succeeded;
}
