// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_Sitting.h"
#include "Components/WidgetComponent.h"
#include "Controller/CAS_EnemyController.h"

ACAS_EnemyCapt_Sitting::ACAS_EnemyCapt_Sitting()
{
}

void ACAS_EnemyCapt_Sitting::ActivateEnemyAbility()
{
	return;
}

void ACAS_EnemyCapt_Sitting::BeginPlay()
{
	Super::BeginPlay();

	if (HpBarWidgetComponent) {
		HpBarWidgetComponent->SetVisibility(false);
	}
}

void ACAS_EnemyCapt_Sitting::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (auto EnemyController = Cast<ACAS_EnemyController>(NewController)) {
		GetMesh()->GetAnimInstance()->Montage_Play(CAS_Montage);
	}
}


