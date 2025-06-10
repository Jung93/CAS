// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "Character/CAS_Player.h"
#include "GAS/CAS_GameplayAbility.h"

// Sets default values for this component's properties
UCAS_QuickSlotWidgetComponent::UCAS_QuickSlotWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCAS_QuickSlotWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCAS_QuickSlotWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCAS_QuickSlotWidgetComponent::AddPlayerAbility(int32 index,const TSubclassOf<class UGameplayAbility>& newAbility)
{
	auto player = Cast<ACAS_Player>(GetOwner());
	if (player->IsValidLowLevel()) {
		auto ASC = Cast<UCAS_AbilitySystemComponent>(player->GetAbilitySystemComponent());
		if (ASC->FindAbilitySpecFromClass(newAbility) == nullptr) {
			auto AbilitySpec = FGameplayAbilitySpec(newAbility);
			ASC->GiveAbility(AbilitySpec);
			PlayerAbilities[index] = newAbility;
			return true;
		}
	}
	return false;
}

void UCAS_QuickSlotWidgetComponent::RemovePlayerAbility()
{

}

FCAS_SlotData UCAS_QuickSlotWidgetComponent::GetAbilityData(int32 index)
{
	FCAS_SlotData Data;
	Data.SlotIndex = index;

	if (PlayerAbilities[index]) {
		auto DefaultObj = PlayerAbilities[index]->GetDefaultObject<UCAS_GameplayAbility>();

		if (DefaultObj->IsValidLowLevel()) {
			UTexture2D* TextureData = DefaultObj->AbilityIcon;
			FName TagData = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();

			Data.SlotTexture = TextureData;
			Data.AbilityTag = TagData;
		}
	}
	else {
		Data.SlotTexture = nullptr;
		Data.AbilityTag = FName(TEXT("None"));
	}

	return Data;
}

void UCAS_QuickSlotWidgetComponent::UpdateQuickSlot(int32 index, FCAS_SlotData slotData)
{
	PlayerAbilities[index] = 
}

int32 UCAS_QuickSlotWidgetComponent::FindEmptyPlayerAbilityIndex()
{
	int32 indexCount = 0;
	for (auto& playerAbility : PlayerAbilities) {
		if (playerAbility == nullptr) {
			return indexCount;
		}
		else {
			indexCount++;
		}
	}

	return -1;
}

