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
    SightConfig->PeripheralVisionAngleDegrees = 75.0f;
    SightConfig->SetMaxAge(1.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightConfig->AutoSuccessRangeFromLastSeenLocation = 5.0f;

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


	BlackBoardComponent = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComponent);
	BehaviorComponent->SetBlackBoard(BlackBoardComponent);
	RunBehaviorTree(BehaviorTree);
}

void ACAS_EnemyController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAS_EnemyController::BeginPlay()
{
	Super::BeginPlay();
	//AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ThisClass::OnTargetPerceptionForgotten);

}

void ACAS_EnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto world = GetWorld();

	if (bUseDebug && world && GetPawn())
	{
		auto ThisPawn = GetPawn();
		FVector center = ThisPawn->GetActorLocation();
		center.Z += 50.0f; 
		
		DrawDebugCircle(GetWorld(), // 월드
			center,					// 중심
			SightConfig->SightRadius,		// 시야 범위
			300,
			FColor::Green,
			false,
			-1,
			0,
			0,
			FVector::RightVector,
			FVector::ForwardVector);
	}
}

void ACAS_EnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (BehaviorComponent->IsBehaviorType(EBehaviorType::Stun)) {
		return;
	}

	auto character = Cast<ACAS_Character>(Actor);

	if (!character) {
		return;
	}

	auto playerController = Cast<ACAS_PlayerController>(character->GetController());

	if (!playerController) {
		return;
	}
	bDebugOn = true;
	AActor* Player = nullptr;

	if (Stimulus.WasSuccessfullySensed()) {
		bool bDetected = character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));

		if (bDetected) {
			BehaviorComponent->ChangeBehaviorType(EBehaviorType::Detect);
			Player = Actor;
		}
		else {
			return;
		}
	}
	else if(!Stimulus.WasSuccessfullySensed()){
		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
		Player = nullptr;
	}
	Blackboard->SetValueAsObject("player", Player);
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
	BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
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
