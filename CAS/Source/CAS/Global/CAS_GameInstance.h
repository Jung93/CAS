// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
public:
	UPROPERTY(VisibleAnywhere, Category = "Setting")
	TMap<FString, FString> GameAssets;

};
