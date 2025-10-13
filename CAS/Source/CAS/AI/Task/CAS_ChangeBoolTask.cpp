// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ChangeBoolTask.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCAS_ChangeBoolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Blackboard = OwnerComp.GetBlackboardComponent();

	bool boolean = Blackboard->GetValueAsBool(BoolKey.SelectedKeyName);

	Blackboard->SetValueAsBool(BoolKey.SelectedKeyName, !boolean);

	return EBTNodeResult::Succeeded;
}
