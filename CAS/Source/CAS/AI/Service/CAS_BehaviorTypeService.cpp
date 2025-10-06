// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CAS_BehaviorTypeService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/CAS_EnemyController.h"
#include "AI/CAS_BehaviorComponent.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Character.h"

UCAS_BehaviorTypeService::UCAS_BehaviorTypeService()
{
}

void UCAS_BehaviorTypeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto EnemyController = Cast<ACAS_EnemyController>(OwnerComp.GetOwner());
	auto BehaviorComponent = EnemyController->GetBehaviorComponent();
	auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto Player = Cast<ACAS_Character>(BlackBoard->GetValueAsObject(PlayerKey.SelectedKeyName));
	auto Enemy = Cast<ACAS_Character>(EnemyController->GetPawn());

	auto EnemyTpye = BlackBoard->GetValueAsEnum("EnemyType");
	auto curType = BehaviorComponent->GetBehaviorType();

	if (BehaviorComponent->IsBehaviorType(EBehaviorType::Death)) {

		return;
	}
	//몽타주가 재생중이면 state 재평가 x 
	if (BlackBoard->GetValueAsBool(IsMontagePlayingKey.SelectedKeyName)) {

		return;
	}
	bool bStunState = Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"));
	//스턴 태그 보유 -> 스턴 몽타주재생
	if (bStunState) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Stun);
		return;
	}

	auto temp1 = BlackBoard->GetValueAsVector("LastHeardLocation");
	auto temp2 = Player;
	//소리가 들렸을 경우 -> 해당 위치까지 가서 확인해보기
	if (BlackBoard->GetValueAsVector("LastHeardLocation") != FVector::ZeroVector && !Player) {
		
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Alert);
		return;
	}

	//플레이어를 감지하지 못한 경우 -> 순찰
	if (!Player) {
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
		return;
	}
	
	//플레이어를 감지 한 경우
	float Distance = Enemy->GetDistanceTo(Player);

	//플레이어를 감지했다가 놓쳐서 마지막에 보였던 장소에 가보는 경우 -> 장소에 가보고 perception으로 다음 행동 판단
	auto LastSeenLocation = BlackBoard->GetValueAsVector("LastSeenLocation");
	if (LastSeenLocation != FVector::ZeroVector) {

		BehaviorComponent->ChangeBehaviorType(EBehaviorType::TargetLost);
		return;
	}

	if (Distance < AttackRange) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Ability);

		return;
	}

	if (Distance <= EnemyController->GetSightRange()) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Trace);

		return;
	}
	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Wait);
	return;

}
