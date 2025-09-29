// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Push.h"
#include "Ability_Task/CAS_Task_Push.h"
#include "Controller/CAS_PlayerController.h"
#include "Character/CAS_Player.h"
#include "InteractionActor/CAS_InteractionCube.h"


UCAS_Ability_Push::UCAS_Ability_Push()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_Push::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(PushMontage);
	if (isPlaying) {
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

}

void UCAS_Ability_Push::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);




	auto Task = UCAS_Task_Push::Task_Push(this, "Push");

	if (Task->IsValidLowLevel())
	{
		Task->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		ReceiveTarget(owner, 1);

	}

	auto player = Cast<ACAS_Player>(GetGameplayTaskAvatar(Task));

	FName sectionName = FName("Start");
	if (player->IsValidLowLevel()) {
		if(player->IsInteracting)
			sectionName = FName("End");
	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", PushMontage, 1.0f, true, sectionName);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_Push::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_Push::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto player = Cast<ACAS_Player>(GetActorInfo().AvatarActor);
	auto cube = Cast<ACAS_InteractionCube>(player->GetInteractingActor());

	if (NotifyName == "PushStart")
	{
		player->IsInteracting = true;
	}
	else if (NotifyName == "PushEnd")
	{
		player->IsInteracting = false;
		player->ClearInteractingActor();

	}

	PlayMontageTask->ReadyForActivation();
}

FActiveGameplayEffectHandle UCAS_Ability_Push::ApplyGamePlayEffectToSelf(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
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

void UCAS_Ability_Push::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!TagEffectClassJump)
		return;

	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
		EffectContextHandle.AddInstigator(PlayerState, nullptr);

		if (AbilitySystemComp->GetActiveGameplayEffect(ActiveEffectHandle) == nullptr)
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(TagEffectClassJump, GetAbilityLevel());
			SpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Interacting")));
			ActiveEffectHandle = ApplyGamePlayEffectToSelf(Target, TagEffectClassJump, TaskLevel, EffectContextHandle, AbilitySystemComp);

			AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Interacting")));







		}
		else
		{
			AbilitySystemComp->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle = FActiveGameplayEffectHandle();
			AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Interacting")));
		}
	}

}

