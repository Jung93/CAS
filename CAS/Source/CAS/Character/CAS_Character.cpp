// Fill out your copyright notice in the Description page of Project Settings.

#include "CAS_Character.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/CAS_GameplayAbility.h"
#include "UI/CAS_Hpbar.h"

// Sets default values
ACAS_Character::ACAS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UCAS_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCAS_AttributeSet>("PlayerAttributeSet");

	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBarWidgetComponent->SetupAttachment(GetMesh());
	HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/HpBarWidget"));
	//
	//if (WidgetClassFinder.Succeeded())
	//{
	//	HpBarWidgetClass = WidgetClassFinder.Class;
	//}
}

// Called when the game starts or when spawned
void ACAS_Character::BeginPlay()
{
	Super::BeginPlay();
	if(HpBarWidgetClass){
	
		HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass);
		auto widget = Cast<UCAS_Hpbar>(HpBarWidgetComponent->GetWidget());
		widget->SetHpCount(HpCount);
	}
}

void ACAS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent();
	AddAbilites();
}

void ACAS_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
	AddAbilites();
}

// Called every frame
void ACAS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ACAS_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACAS_Character::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent) {
		return;
	}
	if (!HasAuthority()) {
		return;
	}
	for (TSubclassOf<UGameplayAbility> abilityClass : DefaultAbilities) {

		FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		AbilitySystemComponent->GiveAbility(abilitySpec);
	}
}

void ACAS_Character::AddAbilites()
{
	UCAS_AbilitySystemComponent* ASC = Cast<UCAS_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ASC) {
		return;
	}
	ASC->AddCharacterAbilities(DefaultAbilities);
}

void ACAS_Character::InitAbilitySystemComponent()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Nomal"));
}

void ACAS_Character::ActivateAbility(const FGameplayTag tag)
{
	AbilitySystemComponent->ActivateAbility(tag);
}

void ACAS_Character::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UCAS_GameplayAbility*>& Abilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, Abilities);
	}
}

bool ACAS_Character::CanActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}



