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
	
	int32 GetPlayerHPCount() { return PlayerHPCount; }
	void SetPlayerHPCount(int32 Count) { PlayerHPCount = Count; }
	void ClearPlayerHPCount() { PlayerHPCount = -1; }

	void SaveGameData_Sync(class ACAS_Player* player, int32 index);
	void SaveGameData_ASync(class ACAS_Player* player, int32 index);
	void LoadGameData_Sync(int32 index);
	void LoadGameData_ASync(int32 index);

	bool IsSaveDataReady() { return CachedSaveGameData.bDataLoadingReady; }
	void ClearCachedData() { CachedSaveGameData = FTempSaveGameData(); }
	void ApplyCachedGameData(ACharacter* Character);
	
	void OpenLoadingLevel();
	void OpenStartLevel();
	//세이브 로드용
	FName GetCachedLevelName() { return CachedSaveGameData.Level; }
	//포탈 , 레벨변경 볼륨용
	FName GetNextLevelName() { return FName(*NextLevelName); }
	void SetNextLevelName(FString LevelName) { NextLevelName = LevelName; }
	void ClearNextLevelName() { NextLevelName.Empty(); }
	void OpenNextLevel(FName LevelName);

	void PlayBgm(int32 bgmIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayNormalBgm(bool isNormal = true);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void CrossFadeMusic(bool isDetected); // 음악 버전 교체 함수

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void ManageTick(float DeltaSecond);

	int32 CurrentSlotIndex = -1;
protected:
	UPROPERTY(VisibleAnywhere, Category = "SaveLoad")
	TMap<FString, FString> GameAssets;
	UPROPERTY(VisibleAnywhere, Category = "LevelConversion")
	TArray<struct FCAS_SlotData> QuickSlotAbilities;

	UFUNCTION()
	void OnLoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame);

	UFUNCTION()
	void OnSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess);


	int32 tempIndex = -1;

	FString NextLevelName;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	//TArray<class USoundCue*> Bgms;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	//class UAudioComponent* BgmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class USoundCue* BgmNormal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class USoundCue* BgmDetected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class UAudioComponent* BgmNormalComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class UAudioComponent* BgmDetectedComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	float BgmTargetVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	float BgmFadeSpeed;

	int32 PlayerHPCount = -1;
private:
	float NextNormalVolume;
	float NextDetectedVolume;

	bool ChangeVolume = false;

private:
	struct FTempSaveGameData
	{
		bool bDataLoadingReady = false;
		int32 PlayerHP = 0;
		FVector PlayerLocation = FVector::ZeroVector;
		TArray<FCAS_SlotData> QuickSlotData;
		FName Level;
	};

	FTempSaveGameData CachedSaveGameData;

};
