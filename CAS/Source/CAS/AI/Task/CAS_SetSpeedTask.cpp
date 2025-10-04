// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_SetSpeedTask.h"
#include "CAS_SetSpeedTask.h"
#include "Controller/CAS_EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAS_SetSpeedTask::UCAS_SetSpeedTask()
{
}

EBTNodeResult::Type UCAS_SetSpeedTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto ThisCharacter = Cast<ACAS_Character>(OwnerComp.GetAIOwner()->GetPawn());

    switch (MoveType)
    {
    case EMoveType::Idle:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
        break;
    case EMoveType::Quiet:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
        break;
    case EMoveType::Walk:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 350.0f;
        break;
    case EMoveType::Sprint:
        ThisCharacter->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
        break;
    }

    return EBTNodeResult::Type();
}

