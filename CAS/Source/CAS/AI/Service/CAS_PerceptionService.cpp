// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CAS_PerceptionService.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CAS_PerceptionService.h"

void UCAS_PerceptionService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto BlackBoard = OwnerComp.GetBlackboardComponent();

}
//GetWorld()->GetTimerManager().ClearTimer(TrackingTimerHandle);
//GetWorld()->GetTimerManager().SetTimer(TrackingTimerHandle, this, &ThisClass::StopTrackingPlayer, TrackingTime, false);
							