// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CAS_PerceptionService.h"
#include "Controller/CAS_EnemyController.h"
#include "Character/CAS_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CAS_PerceptionService.h"

void UCAS_PerceptionService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	BlackBoard = OwnerComp.GetBlackboardComponent();
	
	if (BlackBoard->GetValueAsBool(PlayerLostKey.SelectedKeyName) == true) {
		
		//플레이어가 적을 발견해서 인지는 하고있지만 시야에서 놓친경우
		auto player = BlackBoard->GetValueAsObject(TargetKey.SelectedKeyName);
		//새로운 태스크로 만들어서 플레이어의 마지막 위치를 기록해두고 이동해서 스캔
		BlackBoard->SetValueAsVector(MovePosition.SelectedKeyName,player)
	}
	else {

		GetWorld()->GetTimerManager().SetTimer(TrackingTimerHandle, this, &ThisClass::LostPlayerInfo, TrackingTime, false);
	}



}
void UCAS_PerceptionService::LostPlayerInfo()
{
	BlackBoard->SetValueAsBool("bPlayerDetected", false);
	BlackBoard->SetValueAsObject("Player", nullptr);

	GetWorld()->GetTimerManager().ClearTimer(TrackingTimerHandle);
}
