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
	auto Player = Cast<ACAS_Player>(BlackBoard->GetValueAsObject(PlayerKey.SelectedKeyName));
	auto Enemy = Cast<ACAS_Character>(EnemyController->GetPawn());

	if (EnemyController->bDebugOn) {
		auto curType = BehaviorComponent->GetBehaviorType();
		FString EnumName = UEnum::GetValueAsString(curType);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, EnumName);
	}
	if (BehaviorComponent->IsBehaviorType(EBehaviorType::Death)) {

		return;
	}
	//몽타주가 재생중이면 state 재평가 x 
	if (BlackBoard->GetValueAsBool(IsMontagePlayingKey.SelectedKeyName)) {

		return;
	}
	//idle 상태 -> patrol
	if (BehaviorComponent->IsBehaviorType(EBehaviorType::Wait)) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
		return;
	}
	//반드시 플레이어 시야안에 들어왔다가 나간 경우 -> look around 몽타주 재생 -> wait
	if (BlackBoard->GetValueAsBool("bPlayerMissing")) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
		return;
	}

	bool bStunState = Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"));
	//스턴 태그 보유 -> 스턴 몽타주재생 -> wait
	if (bStunState) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Stun);
		return;
	}
	//반드시 소리가 들렸을 경우 -> 해당 위치까지 가서 확인해보기
	if (BlackBoard->GetValueAsVector("LastHeardLocation") != FVector::ZeroVector) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Alert);
		return;
	}
	//플레이어를 감지하지 못한 경우 -> 순찰
	if (!Player) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
		return;

	}
	//플레이어를 감지 한 경우
	float Distance = Enemy->GetDistanceTo(Player);

	if (Distance < AttackRange) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Ability);

		return;
	}

	if (Distance <= EnemyController->GetSightRange()) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Trace);

		return;
	}



	//if (BlackBoard->GetValueAsBool(IsMontagePlayingKey.SelectedKeyName)) {
	//	
	//	return;
	//}
	//if (BehaviorComponent->IsBehaviorType(EBehaviorType::Missed)) {
	//	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
	//	return;
	//}
	//if (Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"))) {
	//
	//	BlackBoard->SetValueAsBool(IsMontagePlayingKey.SelectedKeyName, true);
	//	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Stun);
	//	return;
	//}
	//if (!Player) {
	//	if(!BehaviorComponent->IsBehaviorType(EBehaviorType::Missed)){
	//		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
	//	}
	//
	//	return;
	//}
	//
	//float Distance = Enemy->GetDistanceTo(Player);
	//
	//if (Distance < AttackRange) {
	//	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Ability);
	//	
	//	return;
	//}
	//
	//if (Distance <= EnemyController->GetSightRange()) {
	//	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Trace);
	//	
	//	return;
	//}
	//
}
