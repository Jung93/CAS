// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ClearVectorKeyTask.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCAS_ClearVectorKeyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Blackboard = OwnerComp.GetBlackboardComponent();

	Blackboard->SetValueAsVector(VectorKey.SelectedKeyName,FVector::ZeroVector);

	return EBTNodeResult::Succeeded;
}
