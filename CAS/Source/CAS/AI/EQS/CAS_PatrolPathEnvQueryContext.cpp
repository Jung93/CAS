// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/CAS_PatrolPathEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Character/CAS_Character.h"
#include "AI/CAS_PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCAS_PatrolPathEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	auto ThisCharacter = Cast<ACAS_Character>(QueryInstance.Owner.Get());
	auto EnemyController = Cast<ACAS_EnemyController>(ThisCharacter->GetController());
	
	if (!EnemyController) {
		return;
	}
	auto Blackboard = EnemyController->GetBlackboardComponent();
	
	ACAS_PatrolPath* PatrolPath = ThisCharacter->GetPatrolPath();
	
	if (PatrolPath)
	{
		int32 Index = Blackboard->GetValueAsInt("NextIndex");
		FVector NextPoint = PatrolPath->GetPatrolPoint(Index);
		UEnvQueryItemType_Point::SetContextHelper(ContextData, NextPoint);
	}
}
