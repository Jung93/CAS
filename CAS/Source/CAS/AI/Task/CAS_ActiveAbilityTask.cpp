// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_ActiveAbilityTask.h"
#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_EnemyCapt.h"
#include "GAS/CAS_GameplayAbility.h"


UCAS_ActiveAbilityTask::UCAS_ActiveAbilityTask()
{
}

EBTNodeResult::Type UCAS_ActiveAbilityTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto curPawn = Cast<ACAS_EnemyCapt>(OwnerComp.GetAIOwner()->GetPawn());

    if (!curPawn->IsValidLowLevel()) {
        return EBTNodeResult::Failed;
    }

    auto temp = curPawn->GetEnemyAbility().Get()->GetDefaultObject<UCAS_GameplayAbility>();
    auto ability = Cast<UCAS_GameplayAbility>(temp);


    if (!ability->IsValidLowLevel()) {
        return EBTNodeResult::Failed;
    }

    FGameplayTag tag = ability->GetAbilityTags().First();

    curPawn->ActivateAbility(tag);
    return EBTNodeResult::Succeeded;

}
