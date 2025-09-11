// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_GameplayCueNotifyStatic.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"
#include "Character/CAS_PlayerState.h"
#include "Perception/AISense_Hearing.h"

UCAS_GameplayCueNotifyStatic::UCAS_GameplayCueNotifyStatic()
{

}

bool UCAS_GameplayCueNotifyStatic::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Param) const
{
	bool result = Super::OnExecute_Implementation(Target, Param);

	auto character = Cast<ACAS_Character>(Target);
	UAbilitySystemComponent* ASC = nullptr;

	if (character->IsValidLowLevel())
	{
		auto playerState = Cast<ACAS_PlayerState>(Target);
		auto player = Cast<ACAS_Player>(character);

		if (playerState->IsValidLowLevel())
			ASC = playerState->GetAbilitySystemComponent();
		else
			ASC = character->GetAbilitySystemComponent();

		for (int32 i = 0; i < Tags.Num(); i++)
		{
			if (ASC->HasMatchingGameplayTag(Tags[i]))
			{
				auto characterAudio = character->GetAudioComponent();

				if (characterAudio != nullptr && characterAudio->IsPlaying())
					characterAudio->Stop();

				auto newAudio = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sounds[i], FVector::ZeroVector);
				character->SetAudioComponent(newAudio);
				
				if(player)
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), player->GetActorLocation(), 1.0f, player, 1000.0f, TEXT("Ability"));
				
				
				break;
			}
		}
	}



	return result;
}


