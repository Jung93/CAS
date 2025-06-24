// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_FireBreath.h"

UCAS_Task_FireBreath* UCAS_Task_FireBreath::Task_FireBreath(UGameplayAbility* OwningAbility, FName TaskName, float Rate)
{
	UCAS_Task_FireBreath* Task = NewAbilityTask<UCAS_Task_FireBreath>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_FireBreath::Activate()
{
	Super::Activate();

	FHitResult hitResult;
	FCollisionQueryParams params(FName("Task_FireBreath"), false, GetAvatarActor());

	FVector forward = GetAvatarActor()->GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward);

	float attackRadius = 60.0f;
	float attackRange = 300.0f;

	FVector Center = GetAvatarActor()->GetActorLocation() + forward * (attackRange * 0.5f);
	FVector Start = GetAvatarActor()->GetActorLocation() + forward * (attackRange * 0.5f);
	FVector End = GetAvatarActor()->GetActorLocation() + forward * (attackRange * 0.5f);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT hitResult,
		Start,
		End,
		quat,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(attackRadius, attackRange * 0.5f),
		params
	);


	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		ACAS_Character* victim = Cast<ACAS_Character>(hitResult.GetActor());
		if (victim) {

			//FVector hitPoint = hitResult.ImpactPoint;			
			OnAttackHit.Broadcast(victim, 1);

		}
	}





	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCAS_Task_FireBreath::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}
