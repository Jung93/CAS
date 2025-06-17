// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Attack.h"
#include "Character/CAS_Character.h"

UCAS_Task_Attack* UCAS_Task_Attack::Task_Attack(UGameplayAbility* OwningAbility, FName TaskName, float AttackRange,float AttackRadius)
{
	UCAS_Task_Attack* Task = NewAbilityTask<UCAS_Task_Attack>(OwningAbility, TaskName);

	Task->AttackRange = AttackRange;
	Task->AttackRadius = AttackRadius;
	return Task;
}

void UCAS_Task_Attack::Activate()
{
	Super::Activate();

	FHitResult hitResult;
	FCollisionQueryParams params(FName("Task_Attack"), false, GetAvatarActor());

	FVector forward = GetAvatarActor()->GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward);

	FVector Center = GetAvatarActor()->GetActorLocation() + forward * AttackRange * 0.5f;
	FVector Start = GetAvatarActor()->GetActorLocation();
	FVector End = GetAvatarActor()->GetActorLocation() + forward * AttackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT hitResult,
		Start,
		End,
		quat,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(AttackRadius, AttackRange * 0.5f),
		params
	);

	FColor drawColor = FColor::Green;

	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		ACAS_Character* victim = Cast<ACAS_Character>(hitResult.GetActor());
		if (victim) {	
			OnAttackHit.Broadcast(victim, 1);
		}
	}


	DrawDebugCapsule(
		GetWorld(),
		Center,
		AttackRange * 0.5f,
		AttackRadius,
		quat,
		drawColor,
		false,
		1.0f
	);

	
}


