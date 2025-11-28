// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyNoCapt/CAS_Dancer.h"
#include "Controller/CAS_EnemyController.h"

ACAS_Dancer::ACAS_Dancer()
{
}

void ACAS_Dancer::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::MontageLoop);

}

void ACAS_Dancer::MontageLoop(UAnimMontage* MontageToLoop, bool IsInterupt)
{
	GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);

}