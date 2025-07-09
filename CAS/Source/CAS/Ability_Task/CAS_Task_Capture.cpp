// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Capture.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"
UCAS_Task_Capture* UCAS_Task_Capture::Task_Capture(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_Capture* Task = NewAbilityTask<UCAS_Task_Capture>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_Capture::Activate()
{
	Super::Activate();


	//UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	//if (ASC)
	//{
	//	auto animInstance = Cast<ACAS_Character>(ASC->GetAvatarActor())->GetMesh()->GetAnimInstance();

	//	animInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCAS_Task_Capture::OnNotifyReceived);
	//}

	auto player = Cast<ACAS_Player>((GetAvatarActor()));
	if (player->IsValidLowLevel()) {

		ACAS_Hat* Hat =  player->GetHat();

		auto dir = player->GetActorForwardVector();

		Hat->Throw(dir);
		Hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


		auto Handle = Ability->GetCurrentAbilitySpecHandle();
		auto ActorInfo = Ability->GetCurrentActorInfo();
		auto ActivationInfo = Ability->GetCurrentActivationInfo();

		AbilityEndEvent.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);

	}
}

void UCAS_Task_Capture::OnNotifyReceived(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == "Capture1")
	{
		auto player = Cast<ACAS_Player>((GetAvatarActor()));
		if (player->IsValidLowLevel()) {

			ACAS_Hat* Hat =  player->GetHat();

			auto dir = player->GetActorForwardVector();

			Hat->Throw(dir);
			Hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
	else if (NotifyName == "Capture2")
	{
		auto Handle = Ability->GetCurrentAbilitySpecHandle();
		auto ActorInfo = Ability->GetCurrentActorInfo();
		auto ActivationInfo = Ability->GetCurrentActivationInfo();

		AbilityEndEvent.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
	}


}
