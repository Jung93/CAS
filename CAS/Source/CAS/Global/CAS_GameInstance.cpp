// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_GameInstance.h"

UCAS_GameInstance::UCAS_GameInstance()
{
}

void UCAS_GameInstance::Init()
{
	Super::Init();

}

FString UCAS_GameInstance::GetConfig(const FString& key, const FString& value) const
{
	return FString();
}

void UCAS_GameInstance::SetConfig(const FString& key, FString& value)
{
}

void UCAS_GameInstance::LoadSettingFromINI()
{
}

void UCAS_GameInstance::SaveSettingToINI()
{
}

FString UCAS_GameInstance::GetSettingPath() const
{
	return FString();
}
