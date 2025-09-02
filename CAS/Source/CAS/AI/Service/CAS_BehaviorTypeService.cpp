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

	if (Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"))) {

		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Stun);

		return;
	}
	if (!Player) {
		if(!BehaviorComponent->IsBehaviorType(EBehaviorType::Missed)){
			BehaviorComponent->ChangeBehaviorType(EBehaviorType::Patrol);
		}

		return;
	}

	float Distance = Enemy->GetDistanceTo(Player);

	if (Distance < AttackRange) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Ability);
		
		return;
	}

	if (Distance <= EnemyController->GetSightRange()) {
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Trace);
		
		return;
	}
	
}
