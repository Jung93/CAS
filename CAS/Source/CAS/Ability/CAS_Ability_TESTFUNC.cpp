// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TESTFUNC.h"
#include "Ability_Task/CAS_Task_Attack.h"

UCAS_Ability_TESTFUNC::UCAS_Ability_TESTFUNC()
{
	//AbilityTags.AddTag(CAS_GamePlayTag::Ability_Attack_TEST); 코드로 작성해도 되지만 블루프린트에서 하는게 더 디자이너 친화적
	notifyName = "Attack_Hit";
}


bool UCAS_Ability_TESTFUNC::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	return true;
}

void UCAS_Ability_TESTFUNC::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*
	1.행동(Task) 실행
	2.효과(Effect) 적용
	*/
	if (!AttackMontage || !DamageEffectClass) {
		return;
	}
	//1.
	/*
	정의할 행동과 기능 
	예를들면 순간적으로 이동하여 공격을 한다면 
	1. 적에게 텔레포트하는 몽타주
	2. 실제 텔레포트 하는 기능
	*/
	auto Task = UCAS_Task_Attack::CAS_Task_Attack(this,"TEST_Attack",AttackMontage,1.5f);
	if (Task->IsValidLowLevel()) {
		Task->ReadyForActivation();
	}


	//2.
	/*
	1.effect : 효과 클래스 그자체
	2.effect spec : 클래스의 세부내용 변경
	3.effect spec handle : 스펙으로 완성한 이펙트를 감싸는 포인터같은 개념 안전하게 참조하는 역할
	예를들면 적에게 공격을할때
	1. 이펙트는 공격하는 그 자체를 정의
	2. 이펙트에서 정의한 공격에 세부적인 데미지 정의 10 을 modifier에 전달
	3. 핸들을 통해 전달하여 적용
	*/
	if (ActorInfo && ActorInfo->OwnerActor.IsValid())
	{
		if (!DamageEffectClass) {
			return;
		}
		auto Character = Cast<ACAS_Character>(ActorInfo->AvatarActor);
		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();

		if (ASC)
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddInstigator(Character, nullptr);

			ApplyGamePlayEffect(Character, DamageEffectClass, 1, EffectContextHandle);

		}
	}

}

void UCAS_Ability_TESTFUNC::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_TESTFUNC::ApplyGamePlayEffect(class ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext)
{
	auto Character = Cast<ACAS_Character>(EffectContext.GetInstigator());
	UAbilitySystemComponent* MyASC = Character->GetAbilitySystemComponent();

	UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
	
	//스펙을 클래스로 받아와서 만들어줌 1.0은 레벨지정 데이터테이블로 고렙 -> 고데미지 스킬로 만들수있음
	FGameplayEffectSpecHandle SpecHandle = MyASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		//스펙핸들에서 스펙을 뽑아와서 태그를 통해 키값을확인해서 데미지입력
		//FGameplayEffectSpec* Spec = SpecHandle.Data.Get(); 이 코드도 사용은 가능하지만 캡슐화 원칙 위배
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST")), -10.0f);
		MyASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, MyASC);
	}
}

void UCAS_Ability_TESTFUNC::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::PlayAnimNotify(NotifyName, BranchingPointPayload);
	if (NotifyName == notifyName) {

	
	}
}


