#include "Ability_Task/CAS_Task_FireBreath.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

UCAS_Task_FireBreath* UCAS_Task_FireBreath::Task_FireBreath(UGameplayAbility* OwningAbility, FName TaskName, float Duration)
{
	UCAS_Task_FireBreath* Task = NewAbilityTask<UCAS_Task_FireBreath>(OwningAbility, TaskName);
	Task->MaxDuration = Duration;
	Task->bTickingTask = true;
	return Task;
}

void UCAS_Task_FireBreath::Activate()
{
	Super::Activate();
}

void UCAS_Task_FireBreath::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	TotalTime += DeltaTime;
	if (!Ability || !GetAvatarActor())
	{
		EndTask();
		return;
	}

	FHitResult hitResult;
	FCollisionQueryParams params(FName("Task_FireBreath"), false, GetAvatarActor());

	FVector forward = GetAvatarActor()->GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward);

	float attackRadius = 40.0f;
	float attackRange = 300.0f;

	FVector Start = GetAvatarActor()->GetActorLocation() + forward * (attackRange/ 2);
	FVector End = Start + forward * attackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT hitResult,
		Start,
		End,
		quat,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(attackRadius, attackRange * 0.5f),
		params
	);


	if (bResult && hitResult.GetActor()->IsValidLowLevel() && !AlreadyHitActors.Contains(hitResult.GetActor()))
	{
		ACAS_Character* victim = Cast<ACAS_Character>(hitResult.GetActor());

		AlreadyHitActors.Add(victim);
		OnAttackHit.Broadcast(victim, 1);
	}

	if (TotalTime >= MaxDuration)
	{
		auto Handle = Ability->GetCurrentAbilitySpecHandle();
		auto ActorInfo = Ability->GetCurrentActorInfo();
		auto ActivationInfo = Ability->GetCurrentActivationInfo();


		AlreadyHitActors.Empty();

		AbilityEndEvent.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
		EndTask();
	}
}
