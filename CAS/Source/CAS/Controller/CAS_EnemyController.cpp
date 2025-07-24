// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_EnemyController.h"
#include "Controller/CAS_PlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"

ACAS_EnemyController::ACAS_EnemyController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1500.0f;
    SightConfig->LoseSightRadius = 1800.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->SetMaxAge(5.0f);

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ACAS_EnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	BlackBoardComponent = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComponent);
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
	if (Actor == nullptr || Cast<ACAS_Character>(Actor) == nullptr) {
		return;
	}
	auto character = Cast<ACAS_Character>(Actor);

	auto CharacterController = character->GetController();

	if (auto playerController = Cast<ACAS_PlayerController>(CharacterController)) {
		bool bDetected = character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
		
		if (bDetected) {
			BlackBoardComponent->SetValueAsBool("bPlayerDetected", true);
			BlackBoardComponent->SetValueAsObject("Player", Actor);
			BlackBoardComponent->SetValueAsVector("MovePosition", Actor->GetActorLocation());
			GetWorld()->GetTimerManager().SetTimer(TrackingTimerHandle, this, &ThisClass::StopTrackingPlayer, TrackingTime, false);
		}
				
	}
	
}

void ACAS_EnemyController::StopTrackingPlayer()
{
	BlackBoardComponent->SetValueAsBool("bPlayerDetected", false);
	BlackBoardComponent->SetValueAsObject("Player", nullptr);
	BlackBoardComponent->SetValueAsVector("MovePosition", GetPawn()->GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(TrackingTimerHandle);
}
