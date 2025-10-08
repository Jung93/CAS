// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/CAS_EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h" 
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/CAS_EnemyController.h"
#include "AI/CAS_BehaviorComponent.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Character.h"
#include "GameFramework/PlayerStart.h"

void UCAS_EnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	auto OwnerActor = Cast<ACAS_Character>(QueryInstance.Owner.Get());
	if (!OwnerActor) {
		return;
	}	

	auto EnemyController = Cast<ACAS_EnemyController>(OwnerActor->GetController());
	if (!EnemyController) {
		return;
	}
	auto Blackboard = EnemyController->GetBlackboardComponent();
	auto PlayerKey = Blackboard->GetValueAsObject("player");

	if (!PlayerKey)
	{
		return;
	}
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<ACAS_Player>(PlayerKey));
}
