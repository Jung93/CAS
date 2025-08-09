// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/CAS_SkillSlot.h"
#include "CAS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCAS_GameInstance();

	virtual void Init() override;

	FString GetSettingPath(const FString& FileName)const;
	void GetAssetsFromPaths(const FString& Path, const FString& HeaderName, TMap<FString, FString>& OutGameAssets);
	void SaveSettingsToINI(const FString& FileName);
	void LoadSettingsFromINI(const FString& FileName);

	void SetQuickSlotSize(int32 size) { QuickSlotAbilities.SetNum(size); }
	void SetQuickSlotAbilityData(int32 index, struct FCAS_SlotData data) { QuickSlotAbilities[index] = data; }
	struct FCAS_SlotData GetQuickSlotAbilityData(int32 index) { return QuickSlotAbilities[index]; }
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "SaveLoad")
	TMap<FString, FString> GameAssets;
	UPROPERTY(VisibleAnywhere, Category = "LevelConversion")
	TArray<struct FCAS_SlotData> QuickSlotAbilities;


};
