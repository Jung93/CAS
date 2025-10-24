// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Dead.h"
#include "Ability_Task/CAS_Task_Dead.h"
#include "Character/CAS_Player.h"
#include "Controller/CAS_PlayerController.h"
#include "LevelManager/CAS_WorldSubsystem.h"

UCAS_Ability_Dead::UCAS_Ability_Dead()
{
}

bool UCAS_Ability_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto AnimInstance = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance();
	auto isPlaying = AnimInstance->Montage_IsPlaying(DeadMontage);
	if (isPlaying) {
		AnimInstance->StopAllMontages(0.1f);
		return true;
	}
	return true;
}

void UCAS_Ability_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!DeadMontage) {
		return;
	}

	PlayMontageTask = UCAS_Task_PlayMontage::Task_PlayMontage(this, "Dead", DeadMontage, 1.0f,true);
	if (PlayMontageTask->IsValidLowLevel()) {
		PlayMontageTask->TaskEndEvent.AddUObject(this, &ThisClass::CAS_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}
}

void UCAS_Ability_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayMontageTask->EndTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
void UCAS_Ability_Dead::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto Character = Cast<ACAS_Character>(GetAvatarActorFromActorInfo());



	auto ASC = Character->GetAbilitySystemComponent();

	if (NotifyName == "DeadSound")
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
		ASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));
	}
	else if(NotifyName == "DeadMontage")
	{
		Character->SetActorHiddenInGame(true);
		//Character->SetActorEnableCollision(false);

		auto playerController = Cast<ACAS_PlayerController>(Character->GetController());

		if (playerController->IsValidLowLevel())
		{
			playerController->ClearDetectingEnemy();
			playerController->OpenTitle();
			PlayMontageTask->TaskEndEvent.Broadcast();
			return;
		}

		auto player = Cast<ACAS_PlayerController>(GetWorld()->GetFirstPlayerController());

		if (player)
		{
			player->RemoveDetectingEnemy(Character);
		}


		Character->Controller->UnPossess();

		auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
		PuzzleSubsystem->PlusCompletedCount();


		PlayMontageTask->TaskEndEvent.Broadcast();
	}



}
