// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_SetSpeedTask.h"
#include "CAS_SetSpeedTask.h"
#include "Controller/CAS_EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CAS_AbilitySystemComponent.h"

UCAS_SetSpeedTask::UCAS_SetSpeedTask()
{
}

EBTNodeResult::Type UCAS_SetSpeedTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto ThisCharacter = Cast<ACAS_Character>(OwnerComp.GetAIOwner()->GetPawn());
    auto ASC = Cast<UCAS_AbilitySystemComponent>(ThisCharacter->GetAbilitySystemComponent());
    auto Check = ASC->FindAbilitySpecByTag(FGameplayTag::RequestGameplayTag("Ability.Move.SuperSpeed"));
    if (Check) {
        return EBTNodeResult::Succeeded; //특수한 이동속도 능력을 가진 캐릭터는 변경하지않는다 
    }
    switch (MoveType)
    {
    case EMoveType::Idle:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
        break;
    case EMoveType::Patrol:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
        break;
    case EMoveType::Walk:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 350.0f;
        break;
    case EMoveType::Sprint:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
        break;
    case EMoveType::PreAttackPhase:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
        break;
    }

    return EBTNodeResult::Succeeded;
}

