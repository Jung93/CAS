// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_DeCapture.h"
#include "Character/CAS_EnemyCapt.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"


UCAS_Task_DeCapture* UCAS_Task_DeCapture::Task_DeCapture(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_DeCapture* Task = NewAbilityTask<UCAS_Task_DeCapture>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_DeCapture::Activate()
{
	Super::Activate();

	auto owner = Cast<ACAS_EnemyCapt>(GetAvatarActor());

	if (!owner->IsValidLowLevel())
		return;

	if (!owner->IsCaptured())
		return;
	
	owner->ActivateCapture(false);
	auto player = owner->GetHat()->GetPlayer();

	auto controller = Cast<APlayerController>(owner->GetController());
	controller->UnPossess();

	controller->Possess(player);

	owner->AddPlayerAbility(player);

	int32 curHp = owner->GetAttributeSet()->GetHealth();

	player->SetHp(curHp);

	int32 HpBeforeCapture = owner->GetHP();
	owner->SetHp(HpBeforeCapture);

	auto iter = GetWorld()->GetControllerIterator();
	for (int i = 0; i < GetWorld()->GetNumControllers(); i++)
	{
		if ((*iter)->GetPawn() != nullptr)
		{
			iter++;
			continue;
		}
		break;
	}

	auto aiController = Cast<AAIController>(*iter);
	aiController->Possess(owner);

	//FVector acotrLocation = owner->GetActorLocation();

	//float dropRadius = 300.0f;
	//FVector randomOffset = FMath::VRand() * FMath::FRandRange(200.0f, dropRadius);
	//FVector dropLocation = acotrLocation + randomOffset;
	//dropLocation.Z = acotrLocation.Z;

	//player->SetActorLocation(dropLocation);

	//-----------------------------------
	FVector actorLocation = owner->GetActorLocation();
	float dropRadius = 300.0f;

	FVector dropLocation = actorLocation;
	bool bFoundValidLocation = false;

	const int MaxAttempts = 10; // 너무 오래 돌지 않도록 제한

	for (int i = 0; i < MaxAttempts; ++i)
	{
		FVector randomOffset = FMath::VRand() * FMath::FRandRange(100.0f, dropRadius);
		FVector candidateLocation = actorLocation + randomOffset;
		candidateLocation.Z = actorLocation.Z;

		// 1. 벽과 겹치는지 확인 (Capsule sweep 사용)
		FCollisionShape capsule = FCollisionShape::MakeCapsule(34.0f, 88.0f); // 플레이어 충돌 캡슐과 동일하게 설정
		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		bool bOverlapsWall = GetWorld()->OverlapBlockingTestByChannel(
			candidateLocation,
			FQuat::Identity,
			ECC_WorldStatic, // 벽이나 구조물에 해당
			capsule,
			params
		);

		if (bOverlapsWall)
			continue;

		// 2. 아래 바닥이 있는지 확인 (Line trace 아래로)
		FVector start = candidateLocation + FVector(0, 0, 50);
		FVector end = candidateLocation - FVector(0, 0, 500); // 충분히 아래까지

		FHitResult hitResult;
		bool bHitFloor = GetWorld()->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_Visibility, // 또는 ECC_WorldStatic
			params
		);

		if (!bHitFloor)
			continue;

		// 유효한 위치 발견
		dropLocation = candidateLocation;
		bFoundValidLocation = true;
		break;
	}

	if (!bFoundValidLocation)
	{
		// 대체 위치: 원래 위치
		dropLocation = actorLocation;
	}

	player->SetActorLocation(dropLocation);
	//-----------------------------------
	player->SetActorHiddenInGame(false);
	player->SetActorEnableCollision(true);


	owner->GetHat()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	owner->GetHat()->Return();
	owner->ClearHat();
	//owner->BeStunned();

	TaskEndEvent.Broadcast();
}
