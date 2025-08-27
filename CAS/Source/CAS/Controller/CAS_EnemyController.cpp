// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_EnemyController.h"
#include "Controller/CAS_PlayerController.h"
#include "AI/CAS_BehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"

ACAS_EnemyController::ACAS_EnemyController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	BehaviorComponent = CreateDefaultSubobject<UCAS_BehaviorComponent>(TEXT("BehaviorComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sense_Sight"));

    SightConfig->SightRadius = 1500.0f;
    SightConfig->LoseSightRadius = 7500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->SetMaxAge(1.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	if (bUseDebug) {
		PrimaryActorTick.bCanEverTick = true;
	}
	else {
		PrimaryActorTick.bCanEverTick = false;
	}
}

void ACAS_EnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	//SetMeshColor(GetPawn(), OriginalColorVector, "Tint");

	BlackBoardComponent = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComponent);
	BehaviorComponent->SetBlackBoard(BlackBoardComponent);
	RunBehaviorTree(BehaviorTree);
}

void ACAS_EnemyController::OnUnPossess()
{
	//SetMeshColor(GetPawn(), OriginalColorVector, "Tint");
	Super::OnUnPossess();
}

void ACAS_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ThisClass::OnTargetPerceptionForgotten);

}

void ACAS_EnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto world = GetWorld();

	if (bUseDebug||!GetPawn()||world)
	{
		auto ThisPawn = GetPawn();
		FVector center = ThisPawn->GetActorLocation();
		center.Z += 50.0f; 
		DrawDebugCone(
			world,
			center,                    
			ThisPawn->GetActorForwardVector(),
			SightConfig->SightRadius,   
			FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees),
			FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees),
			36,
			FColor::Green,
			false,
			-1,
			1,
			1
		);
		DrawDebugCone(
			world,
			center,
			ThisPawn->GetActorForwardVector(),
			SightConfig->LoseSightRadius,
			FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees),
			FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees),
			36,
			FColor::Red,
			false,
			-1,
			0,
			1
		);
	}
}

void ACAS_EnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	/*if (Actor == nullptr || Cast<ACAS_Character>(Actor) == nullptr) {
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
	}*/

}

void ACAS_EnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	ACAS_Player* Player = nullptr;
	TArray<AActor*> Actors;

	AIPerceptionComponent->GetCurrentlyPerceivedActors(nullptr, Actors);

	for (AActor* Actor : Actors)
	{
		Player = Cast<ACAS_Player>(Actor);
		if (Player) {
			int32 temp = 1;
			break;
		}
	}
	Blackboard->SetValueAsObject("player", Player);

}

void ACAS_EnemyController::OnTargetPerceptionForgotten(AActor* Actor)
{
	auto Player = Cast<ACAS_Player>(Actor);
	if (!Player) {
		return;
	}
	Blackboard->SetValueAsEnum("BehaviorType", static_cast<uint8>(EBehaviorType::Missed));
	Blackboard->SetValueAsObject("player", nullptr);
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
