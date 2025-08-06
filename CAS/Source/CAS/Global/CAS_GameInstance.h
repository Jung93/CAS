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

	UFUNCTION()
	FString GetConfig(const FString& key, const FString& value) const;
	UFUNCTION()
	void SetConfig(const FString& key, FString& value);
	UFUNCTION()
	void LoadSettingFromINI();	
	UFUNCTION()
	void SaveSettingToINI();						

	FString GetSettingPath()const;

	UPROPERTY(VisibleAnywhere, Category = "Setting")
	TMap<FString, FString> GameSetting;
};
