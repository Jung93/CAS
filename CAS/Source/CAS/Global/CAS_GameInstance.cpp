// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_GameInstance.h"
#include "Global/CAS_SaveGame.h"
#include "Sound/SoundCue.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CAS_Player.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Components/AudioComponent.h"


UCAS_GameInstance::UCAS_GameInstance()
{
	//ConstructorHelpers::FObjectFinder<USoundBase> bgm(TEXT("/Game/CAS/Resource/Sound/sound_Henesys.sound_Henesys"));
	//if (bgm.Succeeded())
	//{
	//	BackgroundMusic = bgm.Object;
	//}

	BgmComponent = nullptr;

}

void UCAS_GameInstance::Init()
{
	Super::Init();
	GetAssetsFromPaths(TEXT("/Game/CAS/Resource/Texture"), TEXT("[TEXTURE ICON]"), GameAssets);


}

FString UCAS_GameInstance::GetSettingPath(const FString& FileName) const
{
	return FPaths::ProjectSavedDir() + FileName + TEXT(".ini");
}

void UCAS_GameInstance::GetAssetsFromPaths(const FString& Path, const FString& HeaderName, TMap<FString, FString>& OutGameAssets)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> AssetDataList;

	AssetRegistryModule.Get().GetAssetsByPath(FName(*Path), AssetDataList, true);

	OutGameAssets.Add(HeaderName, "");

	for (auto AssetData : AssetDataList) {

		FString AssetName = AssetData.AssetName.ToString();
		FString AssetPath = AssetData.ObjectPath.ToString();

		OutGameAssets.Add(AssetName, AssetPath);
	}
}

void UCAS_GameInstance::SaveSettingsToINI(const FString& FileName)
{
	FString AssetsPath = GetSettingPath(FileName);

	TArray<FString> Contents;

	for (auto GameAsset : GameAssets) {
		FString temp = FString::Printf(TEXT("%s=%s"), *GameAsset.Key, *GameAsset.Value);
		Contents.Add(temp);
	}
	FFileHelper::SaveStringArrayToFile(Contents, *AssetsPath);
}

void UCAS_GameInstance::LoadSettingsFromINI(const FString& FileName)
{
	FString AssetsPath = GetSettingPath(FileName);

	TArray<FString> Contents;
	bool bDataLoadCompleted = FFileHelper::LoadFileToStringArray(Contents, *AssetsPath);

	if (!bDataLoadCompleted) {
		return;
	}
	GameAssets.Empty();

	for (auto Content : Contents) {

		FString Key;
		FString Value;

		if (Content.Split(TEXT("="), &Key, &Value))
		{
			if (!Value.IsEmpty()) {

				GameAssets.Add(Key, Value);
			}
		}
	}

}

void UCAS_GameInstance::SaveGameData_Sync(ACAS_Player* player, int32 index)
{
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCAS_SaveGame::StaticClass()));
	
	if (!player) {
		return;
	}
	player->SaveCharacterData();

	SaveGameData->PlayerHP = player->GetAttributeSet()->GetHealth();
	SaveGameData->PlayerLocation = player->GetActorLocation();
	SaveGameData->QuickSlotData = QuickSlotAbilities;
	SaveGameData->Level = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld(), true));

	UGameplayStatics::SaveGameToSlot(SaveGameData,FString::Printf(TEXT("SLOT_%d"), index), 0);
}

void UCAS_GameInstance::SaveGameData_ASync(ACAS_Player* player, int32 index)
{
}

void UCAS_GameInstance::LoadGameData_Sync(ACAS_Player* player, int32 index)
{
	if (!UGameplayStatics::DoesSaveGameExist(FString::Printf(TEXT("SLOT_%d"), index), 0)) {
		return;
	}
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), index), 0));
	if (!SaveGameData) { 
		return;
	}
	player->GetAttributeSet()->SetHealth(SaveGameData->PlayerHP);
	player->SetActorLocation(SaveGameData->PlayerLocation);
	player->LoadCharacterData();
	
	UGameplayStatics::OpenLevel(GetWorld(), SaveGameData->Level);
}

void UCAS_GameInstance::LoadGameData_ASync(ACAS_Player* player, int32 index)
{
	if (!UGameplayStatics::DoesSaveGameExist(FString::Printf(TEXT("SLOT_%d"), index), 0)) {
		return;
	}
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), index), 0));
	if (!SaveGameData) {
		return;
	}
	tempIndex = index;
	bool bSuccess = false;

	player->GetAttributeSet()->SetHealth(SaveGameData->PlayerHP);
	player->SetActorLocation(SaveGameData->PlayerLocation);
	player->LoadCharacterData();

	ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), SaveGameData->Level.ToString(), FVector::ZeroVector, FRotator::ZeroRotator, bSuccess);
	if (StreamingLevel)
	{	
		StreamingLevel->OnLevelLoaded.AddDynamic(this, &UCAS_GameInstance::LoadLevelEvent);		
	}
}

void UCAS_GameInstance::LoadLevelEvent()
{
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), tempIndex), 0));
	
	UGameplayStatics::OpenLevel(GetWorld(), SaveGameData->Level);

}

void UCAS_GameInstance::PlayBgm(int32 bgmIndex)
{
	if (BgmComponent != nullptr && BgmComponent->IsPlaying())
		BgmComponent->Stop();


	if (Bgms.IsValidIndex(bgmIndex) && Bgms[bgmIndex] != nullptr)
	{
		USoundCue* bgm = Bgms[bgmIndex];
			
		BgmComponent = UGameplayStatics::SpawnSound2D(	// 사운드 생성, 2D로
			GetWorld(),									// 위치(세계)는 로딩된 현재 세계로
			bgm											// 재생할 소리 큐는 가져온 큐로
		);

	}

}

