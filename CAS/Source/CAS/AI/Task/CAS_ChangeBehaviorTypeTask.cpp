// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ChangeBehaviorTypeTask.h"
#include "Controller/CAS_EnemyController.h"

EBTNodeResult::Type UCAS_ChangeBehaviorTypeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto EnemyController = Cast<ACAS_EnemyController>(OwnerComp.GetAIOwner());
	auto BehaviorComponent = EnemyController->GetBehaviorComponent();

	BehaviorComponent->ChangeBehaviorType(BehaviorType);

	return EBTNodeResult::Succeeded;
}
