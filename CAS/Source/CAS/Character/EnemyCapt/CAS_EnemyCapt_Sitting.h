// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_EnemyCapt_Sitting.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt_Sitting : public ACAS_EnemyCapt
{
	GENERATED_BODY()
public:
	ACAS_EnemyCapt_Sitting();

	virtual void ActivateEnemyAbility() override;
protected:


};
