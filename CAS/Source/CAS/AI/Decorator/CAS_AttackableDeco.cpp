// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CAS_AttackableDeco.h"

bool UCAS_AttackableDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    return false;
}
