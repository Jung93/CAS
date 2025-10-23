// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TESTFUNC.h"
#include "Ability_Task/CAS_Task_Attack.h"

UCAS_Ability_TESTFUNC::UCAS_Ability_TESTFUNC()
{		
	//공격당하면 이 어빌리티는 실행되지않음
	//BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}


bool UCAS_Ability_TESTFUNC::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	if (isPlaying) {
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_TESTFUNC::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AttackMontage || !DamageEffectClass) {
		return;
	}

	AttackTask = UCAS_Task_Attack::Task_Attack(this, "Attack", 150.0f, 150.0f);
	//if (AttackTask) {
	//	AttackTask->OnAttackHit.AddUObject(this, &ThisClass::ReceiveTarget);
	//	AttackTask->TaskEndEvent.AddUObject(this, &ThisClass::CAS_EndAbility);
	//}
	if (AttackTask->IsValidLowLevel()) {
		AttackTask->AbilityEndEvent.AddUObject(this, &ThisClass::EndAbility);
		AttackTask->ReadyForActivation();
	}


	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "PlayMontage", AttackMontage, 1.5f,true);
	if (PlayMontageTask) {
		PlayMontageTask->ReadyForActivation();
	}

	PlaySound();

}

void UCAS_Ability_TESTFUNC::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AttackTask->EndTask();
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_TESTFUNC::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		//SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST")), -1.0f);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
	}

}

void UCAS_Ability_TESTFUNC::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!DamageEffectClass) {
		CAS_EndAbility();
	}
	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = nullptr;


	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();

	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
		}
	}
	if (!AbilitySystemComp) {
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
	EffectContextHandle.AddInstigator(PlayerState, nullptr);

	ApplyGamePlayEffect(Target, DamageEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
	//ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);

	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();



	if (PlayerState->IsValidLowLevel())
	{
		TargetAbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));
		//AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Player"));
	}
	else
	{
		TargetAbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Player"));
		//AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));
	};



}

void UCAS_Ability_TESTFUNC::PlaySound()
{
	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = nullptr;



	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();

		auto AttackTag = FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST"));
		AbilitySystemComp->AddLooseGameplayTag(AttackTag);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindLambda([AbilitySystemComp, AttackTag]()
			{
				AbilitySystemComp->RemoveLooseGameplayTag(AttackTag);
			});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.7f, false);

		AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Player"));

	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();

			auto AttackTag = FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST"));
			AbilitySystemComp->AddLooseGameplayTag(AttackTag);

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;

			TimerDelegate.BindLambda([AbilitySystemComp, AttackTag]()
				{
					AbilitySystemComp->RemoveLooseGameplayTag(AttackTag);
				});

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.7f, false);

			AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));

		}
	}


}

void UCAS_Ability_TESTFUNC::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	//AttackTask->ReadyForActivation();
	CAS_EndAbility();
}
