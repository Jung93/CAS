// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_EnemyController.h"
#include "Controller/CAS_PlayerController.h"
#include "AI/CAS_BehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_EnemyCapt.h"
#include "Global/CAS_GameInstance.h"

ACAS_EnemyController::ACAS_EnemyController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	BehaviorComponent = CreateDefaultSubobject<UCAS_BehaviorComponent>(TEXT("BehaviorComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 4500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightConfig->AutoSuccessRangeFromLastSeenLocation = 5.0f;

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	HearingConfig->HearingRange = 2500.0f;
	HearingConfig->SetMaxAge(15.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*HearingConfig);

	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

}

void ACAS_EnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	UseBlackboard(BlackboardData, BlackBoardComponent);
	BehaviorComponent->SetBlackBoard(BlackBoardComponent);
	RunBehaviorTree(BehaviorTree);

	auto ThisPawn = Cast<ACAS_Character>(GetPawn());
	EEnemyType ThisPawnType = ThisPawn->GetEnemyType();
	BlackBoardComponent->SetValueAsEnum("EnemyType", static_cast<uint8>(ThisPawnType));
}

void ACAS_EnemyController::OnUnPossess()
{
	auto ThisCharacter = Cast<ACharacter>(GetPawn());
	auto AnimInstance = ThisCharacter->GetMesh()->GetAnimInstance();

	AnimInstance->StopAllMontages(0.1f);

	Super::OnUnPossess();

}

void ACAS_EnemyController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);

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

	auto ASC = character->GetAbilitySystemComponent();

	if (ASC == nullptr)
		return;

	bool bStunState = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Status.Stun"));
	if (BehaviorComponent->IsBehaviorType(EBehaviorType::Stun) || bStunState) {
		return;
	}

	auto playerController = Cast<ACAS_PlayerController>(character->GetController());

	if (!playerController) {
		return;
	}
	bool bDetectable = character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Detectable"));
	auto CurrBehaviorType = BehaviorComponent->GetBehaviorType();

	FAISenseID StimulusID = Stimulus.Type;

	bDebugOn = true;

	if (Stimulus.WasSuccessfullySensed()) {

		APawn* ThisPawn = GetPawn();
		if (!ThisPawn) {
			return;
		}
		if (StimulusID == SightConfig->GetSenseID()) {
			if (CurrBehaviorType == EBehaviorType::TargetLost) {
				//플레이어가 숨어서 놓쳤다가 해당 위치에 가봤더니 다시 발견한 상황 -> LastSeenLocation를 초기화해서 서비스에서 갱신 막기
				BlackBoardComponent->SetValueAsVector("LastSeenLocation", FVector::ZeroVector);
			}
			if (bDetectable) {
				BehaviorComponent->ChangeBehaviorType(EBehaviorType::Detect);
				BlackBoardComponent->SetValueAsObject("player", Actor);

				bool firstDetection = !playerController->IsAnyDetectingEnemy();

				if (firstDetection)
				{
					UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(Actor->GetGameInstance());
					if (gi)
					{
						gi->CrossFadeMusic(true);
					}
				}
				ACAS_Character* thisCharacter = Cast<ACAS_Character>(ThisPawn);

				playerController->AddDetectingEnemy(thisCharacter);

				return;
			}
			else {
				//시야에 플레이어 컨트롤러가 빙의된 캐릭터가 포착되었으나 탐지가능 태그가 없는경우
				return;
			}
		}
		else if (StimulusID == HearingConfig->GetSenseID()) {
			FVector CurrentLocation = ThisPawn->GetActorLocation();
			FVector TargetLocation = Actor->GetActorLocation();
			FVector TargetVector = TargetLocation - CurrentLocation;
			TargetVector.Z = 0.0f;
			FRotator NewRot = TargetVector.Rotation();

			ThisPawn->SetActorRotation(NewRot);
			BlackBoardComponent->SetValueAsVector("LastHeardLocation", Stimulus.StimulusLocation);
			return;
		}

	}
	else if (!Stimulus.WasSuccessfullySensed()) {
		if (StimulusID == SightConfig->GetSenseID()) {
			BlackBoardComponent->ClearValue("player");
			//BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
			BlackBoardComponent->SetValueAsVector("LastSeenLocation", Stimulus.StimulusLocation);

			ACAS_Character* thisCharacter = Cast<ACAS_Character>(GetPawn());

			playerController->RemoveDetectingEnemy(thisCharacter);

			bool lastDetection = !playerController->IsAnyDetectingEnemy();
			if (lastDetection)
			{
				UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(Actor->GetGameInstance());
				if (gi)
				{
					gi->CrossFadeMusic(false);
				}
			}
		}
		else if (StimulusID == HearingConfig->GetSenseID()) {
			BlackBoardComponent->SetValueAsVector("LastHeardLocation", FVector::ZeroVector);
		}

	}


	//if (StimulusID == HearingConfig->GetSenseID()) {
	//
	//	if (Stimulus.WasSuccessfullySensed()) {
	//		APawn* ThisPawn = GetPawn();
	//		if (!ThisPawn) {
	//			return;
	//		}
	//		FVector CurrentLocation = ThisPawn->GetActorLocation();
	//		FVector TargetLocation = Actor->GetActorLocation();
	//		FVector TargetVector = TargetLocation - CurrentLocation;
	//		TargetVector.Z = 0.0f;
	//		FRotator NewRot = TargetVector.Rotation();
	//
	//		ThisPawn->SetActorRotation(NewRot);
	//		BlackBoardComponent->SetValueAsVector("LastHeardLocation", Stimulus.StimulusLocation);
	//		if (bDetectable) {
	//			BehaviorComponent->ChangeBehaviorType(EBehaviorType::Detect);
	//			Player = Actor;
	//		}
	//	}
	//	else if (!Stimulus.WasSuccessfullySensed()) {
	//		BlackBoardComponent->SetValueAsVector("LastHeardLocation",FVector::ZeroVector);
	//		BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
	//	}
	//}
	//else if (StimulusID == SightConfig->GetSenseID()) {
	//			
	//	if (bDetectable) {
	//		if (Stimulus.WasSuccessfullySensed()) {
	//			APawn* ThisPawn = GetPawn();
	//			if (!ThisPawn) {
	//				return;
	//			}
	//			FVector CurrentLocation = ThisPawn->GetActorLocation();
	//			FVector TargetLocation = Actor->GetActorLocation();
	//			FVector TargetVector = TargetLocation - CurrentLocation;
	//			TargetVector.Z = 0.0f;
	//			FRotator NewRot = TargetVector.Rotation();
	//
	//			ThisPawn->SetActorRotation(NewRot);
	//
	//			BehaviorComponent->ChangeBehaviorType(EBehaviorType::Detect);
	//			Player = Actor;
	//
	//			auto Controller = Cast<ACAS_Character>(Player)->GetController();
	//			auto PlayerController = Cast<ACAS_PlayerController>(Controller);
	//			bool firstDetection = !PlayerController->IsAnyDetectingEnemy();
	//
	//			if (firstDetection)
	//			{
	//				UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(Actor->GetGameInstance());
	//				if (gi)
	//				{
	//					gi->CrossFadeMusic(true);
	//				}
	//			}
	//			ACAS_Character* thisCharacter = Cast<ACAS_Character>(ThisPawn);
	//
	//			PlayerController->AddDetectingEnemy(thisCharacter);
	//
	//		}
	//		else if (!Stimulus.WasSuccessfullySensed()) {
	//			BehaviorComponent->ChangeBehaviorType(EBehaviorType::Missed);
	//			Player = Actor;
	//
	//			auto Controller = Cast<ACAS_Character>(Player)->GetController();
	//			auto PlayerController = Cast<ACAS_PlayerController>(Controller);
	//
	//			ACAS_Character* thisCharacter = Cast<ACAS_Character>(GetPawn());
	//
	//			PlayerController->RemoveDetectingEnemy(thisCharacter);
	//
	//			bool lastDetection = !PlayerController->IsAnyDetectingEnemy();
	//			if (lastDetection)
	//			{
	//				UCAS_GameInstance* gi = Cast<UCAS_GameInstance>(Actor->GetGameInstance());
	//				if (gi)
	//				{
	//					gi->CrossFadeMusic(false);
	//				}
	//			}
	//
	//		}
	//	}
	//	else {
	//		return;
	//	}
	//	BlackBoardComponent->SetValueAsObject("player", Player);
	//	return;
	//}

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