// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_HitScan.h"
#include "Components/SphereComponent.h"
#include "CAS/Character/CAS_Character.h"
#include "CAS/Character/CAS_EnemyCapt.h"
#include "CAS/Character/CAS_Player.h"
#include "CAS/Character/CAS_PlayerState.h"


// Sets default values
ACAS_HitScan::ACAS_HitScan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	RootComponent = Collision;
}

// Called when the game starts or when spawned
void ACAS_HitScan::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentEndOverlap.AddDynamic(this, &ACAS_HitScan::NotifyCollision);


}

// Called every frame
void ACAS_HitScan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), Collision->GetScaledSphereRadius(), 10, FColor::Green, false, 0.1f);

}


void ACAS_HitScan::NotifyCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OwnerClass)
		return;

	auto character = Cast<ACAS_Character>(OtherActor);

	if (!character->IsValidLowLevel() || character == nullptr)
		return;

	auto player = Cast<ACAS_Player>(OwnerClass);
	UAbilitySystemComponent* ASC = nullptr;

	if (player != nullptr)
	{
		if(!player->IsDead())
			ASC = Cast<ACAS_PlayerState>(player->GetPlayerState())->GetAbilitySystemComponent();
	}
	else
	{
		ASC = OwnerClass->GetAbilitySystemComponent();
	}

	if (!ASC->IsValidLowLevel())
		return;

	if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Attack.Test")))
		return;


	character->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.TakeDamage"));

}



