// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CAS_FindPlayerService.h"

#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Player.h"
#include "Engine/OverlapResult.h"

void UCAS_FindPlayerService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto curPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!curPawn->IsValidLowLevel()){
		return;
	}

	FVector pos = curPawn->GetActorLocation();
	float sphereRaidus = 1000.0f;

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams qParams(NAME_None, false, curPawn);

	bool result = GetWorld()->OverlapMultiByChannel(
		overlapResults,
		pos,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(sphereRaidus),
		qParams
	);

	DrawDebugSphere(GetWorld(), pos, sphereRaidus, 30, FColor::Green, false, 0.3f);

	if (!result)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Player")), nullptr);
		return;
	}
	else
	{
		for (auto& overlapResult : overlapResults)
		{
			auto player = Cast<ACAS_Player>(overlapResult.GetActor());
			
			if (player->IsValidLowLevel())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Player")), player);
				DrawDebugSphere(GetWorld(), pos, sphereRaidus, 30, FColor::Red, false, 0.3f);
				return;
			}
		}
	}


}
