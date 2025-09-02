// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ChangeBehaviorType.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UCAS_ChangeBehaviorType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

	BlackBoard->SetValueAsEnum(BehaviorTypeKey.SelectedKeyName, static_cast<uint8>(BehaviorType));

	return EBTNodeResult::Succeeded;
}

