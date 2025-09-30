// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_PickUp.h"
#include "Ability_Task/CAS_Task_PickUp.h"
#include "Controller/CAS_PlayerController.h"
#include "Character/CAS_Player.h"
#include "InteractionActor/CAS_InteractionBall.h"

UCAS_Ability_PickUp::UCAS_Ability_PickUp()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_PickUp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(PickMontage);
	if (isPlaying) {
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

}

void UCAS_Ability_PickUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_PickUp::Task_PickUp(this, "PickUp");

	if (Task->IsValidLowLevel())
	{
		Task->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		ReceiveTarget(owner, 1);

	}
	auto player = Cast<ACAS_Player>(GetActorInfo().AvatarActor);
	FName name = !player->IsInteracting ? FName("PickUp") : FName("PutDown");

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", PickMontage, 1.0f, true, name);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_PickUp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_PickUp::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto player = Cast<ACAS_Player>(GetActorInfo().AvatarActor);
	auto ball = Cast<ACAS_InteractionBall>(player->GetInteractingActor());

	if (NotifyName == FName("PickUp"))
	{
		player->IsInteracting = true;

		ball->GetMesh()->SetSimulatePhysics(false);
		ball->SetActorEnableCollision(false);

		ball->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("index_02_r"));
	}
	else if (NotifyName == FName("PutDown"))
	{
		ball->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		ball->SetActorEnableCollision(true);
		ball->GetMesh()->SetSimulatePhysics(true);
		
		FVector ballLocation;
		FVector PlayerLocation = player->GetActorLocation();

		if (player->bPositionReceived) {
			ballLocation = player->ReceivedPosition;
			player->bPositionReceived = false;
		}
		else {
			ballLocation = PlayerLocation + (player->GetActorForwardVector() * 100.0f);
		}

		ball->SetActorLocation(ballLocation);

		player->IsInteracting = false;
		player->ClearInteractingActor();
	}



	PlayMontageTask->ReadyForActivation();
}

FActiveGameplayEffectHandle UCAS_Ability_PickUp::ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
		return Handle;
	}

	return FActiveGameplayEffectHandle();
}

void UCAS_Ability_PickUp::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
}
