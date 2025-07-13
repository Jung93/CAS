// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "Character/CAS_Player.h"

// Sets default values for this component's properties
UCAS_QuickSlotWidgetComponent::UCAS_QuickSlotWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCAS_QuickSlotWidgetComponent::InitSetting(int32 count)
{
	PlayerAbilities.SetNum(count);

	for (int32 i = 0; i < count; i++) {
		FCAS_SlotData Data;
		Data.SlotIndex = i;
		Data.SkillData.AbilityIconTexture = nullptr;
		Data.SkillData.AbilityTag = FName(TEXT("None"));
		PlayerAbilities[i] = Data;
	}
}

bool UCAS_QuickSlotWidgetComponent::AddPlayerAbility(int32 index,const TSubclassOf<class UGameplayAbility>& newAbility)
{
	auto owner = GetOwner();
	auto player = Cast<ACAS_Player>(GetOwner());
	if (player->IsValidLowLevel()) {
		auto ASC = Cast<UCAS_AbilitySystemComponent>(player->GetAbilitySystemComponent());


		if (ASC->FindAbilitySpecFromClass(newAbility) == nullptr) {
			auto DefaultObj = newAbility->GetDefaultObject<UCAS_GameplayAbility>();
			if (DefaultObj->IsValidLowLevel()) {
				
				//FCAS_SkillData SkillData = DefaultObj->GetSkillData();

				FCAS_SkillData SkillData;
				SkillData.AbilityIconTexture = DefaultObj->AbilityIcon;
				SkillData.AbilityTag = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();
				SkillData.InputID = DefaultObj->InputID;

				auto AbilitySpec = FGameplayAbilitySpec(newAbility, 1, static_cast<int32>(SkillData.InputID));
				ASC->GiveAbility(AbilitySpec);

				FCAS_SlotData SlotData;
				SlotData.SlotIndex = index;
				SlotData.SkillData = SkillData;

				PlayerAbilities[index] = SlotData;

				return true;
			}
			
			
		}

	}
	return false;
}

void UCAS_QuickSlotWidgetComponent::RemovePlayerAbility(int32 index)
{
	FCAS_SlotData slotData = PlayerAbilities[index];
	if (slotData.SkillData.AbilityIconTexture == nullptr) {
		return;
	}

	auto player = Cast<ACAS_Player>(GetOwner());
	if (player->IsValidLowLevel()) {
		auto ASC = Cast<UCAS_AbilitySystemComponent>(player->GetAbilitySystemComponent());
		
		FName tagName = slotData.SkillData.AbilityTag;

		ASC->RemoveAbility(FGameplayTag::RequestGameplayTag(tagName));

		slotData.SlotIndex = index;
		slotData.SkillData.AbilityIconTexture = nullptr;
		slotData.SkillData.AbilityTag = FName(TEXT("None"));

		PlayerAbilities[index] = slotData;
	}
}

FCAS_SlotData UCAS_QuickSlotWidgetComponent::GetAbilityData(int32 index)
{
	return PlayerAbilities[index];
}

void UCAS_QuickSlotWidgetComponent::UpdateQuickSlot(int32 index, FCAS_SlotData slotData)
{
	PlayerAbilities[index] = slotData;
}

int32 UCAS_QuickSlotWidgetComponent::FindEmptyPlayerAbilityIndex()
{
	int32 indexCount = 0;
	for (auto& playerAbility : PlayerAbilities) {
		if (playerAbility.SkillData.AbilityIconTexture == nullptr) {
			return indexCount;
		}
		else {
			indexCount++;
		}
	}

	return -1;
}

