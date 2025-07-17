// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_EnemyController.h"
#include "Controller/CAS_PlayerController.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"

ACAS_EnemyController::ACAS_EnemyController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1500.0f;
    SightConfig->LoseSightRadius = 1800.0f;
    SightConfig->PeripheralVisionAngleDegrees = 30.0f;
    SightConfig->SetMaxAge(3.0f);

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ACAS_EnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UBlackboardComponent* BlackBoardComp = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComp);
	RunBehaviorTree(BehaviorTree);
}

void ACAS_EnemyController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAS_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
}

void ACAS_EnemyController::RandMove()
{
}

void ACAS_EnemyController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	/*
	어빌리티 사용시 몇초간 태그가 붙음(state.detectable)
	태그가 붙은 상태면 인식가능
	
	*/
	if (Actor == nullptr || Cast<ACAS_Character>(Actor) == nullptr) {
		return;
	}
	auto character = Cast<ACAS_Character>(Actor);

	auto CharacterController = character->GetController();

	if (auto playerController = Cast<ACAS_PlayerController>(CharacterController)) {
		//character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
		/*
		1.플레이어 자체인경우 
		2.빙의된 enemy인 경우

		*/
	}
}
