// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UCAS_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->ClearValue(BlackbaordKey.SelectedKeyName);
	if (BlackbaordKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		Blackboard->SetValueAsVector(BlackbaordKey.SelectedKeyName, FVector::ZeroVector);
	}
	return EBTNodeResult::Succeeded;
}
