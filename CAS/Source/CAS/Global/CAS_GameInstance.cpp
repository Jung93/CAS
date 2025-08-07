// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_GameInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

UCAS_GameInstance::UCAS_GameInstance()
{
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

