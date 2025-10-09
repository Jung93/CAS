// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_AnimInstance.h"
#include "Character/CAS_Character.h"
#include "Controller/CAS_EnemyController.h"
#include "Controller/CAS_PlayerController.h"

void UCAS_AnimInstance::CheckControllerClass(AController* Controller)
{
	auto ThisCharacter = Cast<ACAS_Character>(GetOwningActor());

	if (!ThisCharacter || !Controller) {
		return;
	}
	
	if (auto EnemyController = Cast<ACAS_EnemyController>(Controller)) {
		bPlayerControl = false;		
	}

	if (auto PlayerController = Cast<ACAS_PlayerController>(Controller)) {
		bPlayerControl = true;
		bAttackMode = false;
	}
}
