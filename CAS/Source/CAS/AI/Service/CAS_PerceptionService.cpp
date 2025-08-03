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
		//컨트롤러에서 마지막 시야에 포착된 플레이어의 위치정보를 입력
		//두리번거리는 애니메이션 + 5초간 재탐색 -> 플레이어 놓침

	}
	else {
		auto ThisPawn = OwnerComp.GetOwner();
		FVector TargetPoint = Cast<AActor>(BlackBoard->GetValueAsObject(TargetKey.SelectedKeyName))->GetActorLocation();
		FVector TargetRotation = TargetPoint - ThisPawn->GetActorLocation();
		FRotator LookAtRot = FRotationMatrix::MakeFromX(TargetRotation).Rotator();

		ThisPawn->SetActorRotation(LookAtRot);

		GetWorld()->GetTimerManager().SetTimer(TrackingTimerHandle, this, &ThisClass::LostPlayerInfo, TrackingTime, false);
	
	}

}
void UCAS_PerceptionService::LostPlayerInfo()
{
	BlackBoard->SetValueAsBool(PlayerDetectedKey.SelectedKeyName, false);
	BlackBoard->SetValueAsObject(TargetKey.SelectedKeyName, nullptr);

}
