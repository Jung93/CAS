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
	SetMeshColor(GetPawn(), OriginalColorVector, "Tint");
	BlackBoardComponent = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComponent);
	RunBehaviorTree(BehaviorTree);
}

void ACAS_EnemyController::OnUnPossess()
{
	SetMeshColor(GetPawn(), OriginalColorVector, "Tint");
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
	auto NPCpawn = GetPawn();
	auto NPC = Cast<ACAS_Character>(NPCpawn);
	if (NPCpawn == nullptr || NPC->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"))) {
		return;
	}
	auto character = Cast<ACAS_Character>(Actor);

	auto CharacterController = character->GetController();
	auto playerController = Cast<ACAS_PlayerController>(CharacterController);

	if (!playerController) {
		return;
	}

	if (Stimulus.WasSuccessfullySensed()) {
		bool bDetected = character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));

		if (bDetected) {
			BlackBoardComponent->SetValueAsBool("bPlayerDetected", true);
			BlackBoardComponent->SetValueAsObject("Player", Actor);
			BlackBoardComponent->SetValueAsVector("MovePosition", Actor->GetActorLocation());
			BlackBoardComponent->SetValueAsBool("bPlayerLost", false);

			FVector ColorVector(1, 0, 1);
			SetMeshColor(character, ColorVector);

		}
	}
	else {
		FVector ColorVector(0, 0, 0);
		SetMeshColor(character, ColorVector);
		BlackBoardComponent->SetValueAsBool("bPlayerLost", true);
	}

}

void ACAS_EnemyController::SetMeshColor(APawn* pawn, FVector colorVector, FName name)
{
	auto Mesh = Cast<ACharacter>(pawn)->GetMesh();

	UMaterialInstanceDynamic* DynMat0 = Mesh->CreateDynamicMaterialInstance(0);
	if (DynMat0)
	{
		DynMat0->SetVectorParameterValue(name, colorVector);
	}

	UMaterialInstanceDynamic* DynMat1 = Mesh->CreateDynamicMaterialInstance(1);
	if (DynMat1)
	{
		DynMat1->SetVectorParameterValue(name, colorVector);
	}
}
