// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ActivateAbilityTask.h"
#include "Character/CAS_EnemyCapt.h"
#include "Controller/CAS_EnemyController.h"

EBTNodeResult::Type UCAS_ActivateAbilityTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Enemy = Cast<ACAS_EnemyCapt>(AIController->GetPawn());
	
	Enemy->ActivateEnemyAbility();

	return EBTNodeResult::Succeeded;
}
