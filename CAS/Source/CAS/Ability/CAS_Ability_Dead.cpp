// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Dead.h"
#include "Ability_Task/CAS_Task_Dead.h"
#include "Character/CAS_Player.h"
#include "Controller/CAS_PlayerController.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Global/CAS_GameInstance.h"

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

		//Gameplaycue에 등록된 소리 재생
		if (Cast<ACAS_Player>(Character))
			ASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Player"));
		else
			ASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Sound.Enemy"));
	}
	else if(NotifyName == "DeadMontage")
	{
		Character->SetActorHiddenInGame(true);
		//Character->SetActorEnableCollision(false);

		auto playerController = Cast<ACAS_PlayerController>(Character->GetController());

		if (playerController->IsValidLowLevel())
		{
			//플레이어가 죽을 경우
			playerController->ClearDetectingEnemy();
			playerController->OpenTitle();
			PlayMontageTask->TaskEndEvent.Broadcast();
			return;
		}

		//Enemy가 죽을 경우 
		auto player = Cast<ACAS_PlayerController>(GetWorld()->GetFirstPlayerController());

		if (player)
		{
			//플레이어 감지 제거
			player->RemoveDetectingEnemy(Character);

			//플레이어 감지한 Enemy가 없을 경우 배경음악 변경
			if (!player->IsAnyDetectingEnemy())
			{
				UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(player->GetGameInstance());
				if (gi)
				{
					gi->CrossFadeMusic(false);
				}
			}

		}


		//Character->Controller->UnPossess();

		auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
		PuzzleSubsystem->PlusCompletedCount();


		PlayMontageTask->TaskEndEvent.Broadcast();
	}



}
