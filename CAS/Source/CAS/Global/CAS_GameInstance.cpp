// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/CAS_GameInstance.h"
#include "Global/CAS_SaveGame.h"
#include "Sound/SoundCue.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CAS_Player.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Components/AudioComponent.h"

//#include "Math/UnrealMathUtility.h"


UCAS_GameInstance::UCAS_GameInstance()
{
	//ConstructorHelpers::FObjectFinder<USoundBase> bgm(TEXT("/Game/CAS/Resource/Sound/sound_Henesys.sound_Henesys"));
	//if (bgm.Succeeded())
	//{
	//	BackgroundMusic = bgm.Object;
	//}

	//BgmComponent = nullptr;

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
		FString AssetPath = AssetData.GetObjectPathString();

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
	SaveGameData->SaveTime = FDateTime::Now();

	UGameplayStatics::SaveGameToSlot(SaveGameData,FString::Printf(TEXT("SLOT_%d"), index), 0);
}

void UCAS_GameInstance::SaveGameData_ASync(ACAS_Player* player, int32 index)
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
	SaveGameData->SaveTime = FDateTime::Now();

	UGameplayStatics::AsyncSaveGameToSlot(SaveGameData, FString::Printf(TEXT("SLOT_%d"),index), 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &ThisClass::OnSaveFinished));

}

void UCAS_GameInstance::LoadGameData_Sync(int32 index)
{
	if (!UGameplayStatics::DoesSaveGameExist(FString::Printf(TEXT("SLOT_%d"), index), 0)) {
		return;
	}
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), index), 0));
	if (!SaveGameData) { 
		return;
	}

	CachedSaveGameData.PlayerHP = SaveGameData->PlayerHP;
	CachedSaveGameData.PlayerLocation = SaveGameData->PlayerLocation;
	CachedSaveGameData.QuickSlotData = SaveGameData->QuickSlotData;
	CachedSaveGameData.bDataLoadingReady = true;


	UGameplayStatics::OpenLevel(GetWorld(), SaveGameData->Level);
}

void UCAS_GameInstance::LoadGameData_ASync(int32 index)
{
	if (!UGameplayStatics::DoesSaveGameExist(FString::Printf(TEXT("SLOT_%d"), index), 0)) {
		return;
	}
	
	OpenLoadingLevel();

	UGameplayStatics::AsyncLoadGameFromSlot(FString::Printf(TEXT("SLOT_%d"), index), 0, FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &ThisClass::OnLoadFinished));
}

void UCAS_GameInstance::OnLoadFinished(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame)
{
	UCAS_SaveGame* SaveGameData = Cast<UCAS_SaveGame>(LoadedGame);

	CachedSaveGameData.PlayerHP = SaveGameData->PlayerHP;
	CachedSaveGameData.PlayerLocation = SaveGameData->PlayerLocation;
	CachedSaveGameData.QuickSlotData = SaveGameData->QuickSlotData;
	CachedSaveGameData.Level = SaveGameData->Level;
	CachedSaveGameData.bDataLoadingReady = true;
}

void UCAS_GameInstance::OnSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	//성공 실패시 메세지 띄우기 
}

void UCAS_GameInstance::ApplyCachedGameData(ACharacter* Character)
{
	auto SaveLocation = CachedSaveGameData.PlayerLocation;
	auto SaveHP = CachedSaveGameData.PlayerHP;
	QuickSlotAbilities =  CachedSaveGameData.QuickSlotData;

	auto Player = Cast<ACAS_Player>(Character);

	if (Player) {
		Player->SetActorLocation(SaveLocation);
		Player->SetHp(SaveHP);
	}
}

void UCAS_GameInstance::OpenLoadingLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LoadingOnly");
}

void UCAS_GameInstance::OpenStartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Test");
}

void UCAS_GameInstance::OpenNextLevel(FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	ClearNextLevelName();
}

void UCAS_GameInstance::PlayBgm(int32 bgmIndex)
{
	//if (BgmComponent != nullptr && BgmComponent->IsPlaying())
	//	BgmComponent->Stop();


	//if (Bgms.IsValidIndex(bgmIndex) && Bgms[bgmIndex] != nullptr)
	//{
	//	USoundCue* bgm = Bgms[bgmIndex];
	//		
	//	BgmComponent = UGameplayStatics::SpawnSound2D(	// 사운드 생성, 2D로
	//		GetWorld(),									// 위치(세계)는 로딩된 현재 세계로
	//		bgm											// 재생할 소리 큐는 가져온 큐로
	//	);

	//}

}

void UCAS_GameInstance::PlayNormalBgm(bool isDetected)
{
	NextNormalVolume = BgmTargetVolume * ((float)isDetected); // 참이면 x1, 거짓이면 x0
	NextDetectedVolume = BgmTargetVolume * (1 - (float)isDetected); // 참이면 x0, 거짓일 때 x1

	// 사운드 큐가 제대로 설정되었는지 확인
	//if (BgmNormal && BgmDetected)
	if(BgmNormal && BgmDetected)
	{
		// 각 음악을 생성하고 재생

		BgmNormalComponent = UGameplayStatics::SpawnSound2D(GetWorld(),
			BgmNormal);					//  일단 생성 (흔히 사용됨)
		//bgmCalm, nextCalmVolume);		// 옵션 세부 적용 (잘 안 씀)

	// 옵션이 많은 컴포넌트면 만들고 나서 옵션을 적용하는 편이 더 좋다
		if (BgmNormalComponent)
			BgmNormalComponent->SetVolumeMultiplier(NextNormalVolume);

		BgmDetectedComponent = UGameplayStatics::SpawnSound2D(GetWorld(),
			BgmDetected);

		if (BgmDetectedComponent)
			BgmDetectedComponent->SetVolumeMultiplier(NextDetectedVolume);
	}

}

void UCAS_GameInstance::CrossFadeMusic(bool isDetected)
{
	if (isDetected)
	{
		NextDetectedVolume = BgmTargetVolume;
		NextNormalVolume = 0;
	}
	else
	{
		NextNormalVolume = BgmTargetVolume;
		NextDetectedVolume = 0;
	}

	ChangeVolume = true;
}

void UCAS_GameInstance::ManageTick(float DeltaSecond)
{
	if (ChangeVolume)
	{
		if (BgmNormalComponent && BgmDetectedComponent)
		{
			float lerpNormalVolume = FMath::FInterpTo(		// 고급 선형보간
				BgmNormalComponent->VolumeMultiplier,			// 처음
				NextNormalVolume,								// 끝
				DeltaSecond,								// 틱 간격 (보간 정도)
				BgmFadeSpeed								// 보간 속도 (기간에 영향)
			);

			float lerpDetectedVolume = FMath::FInterpTo(
				BgmDetectedComponent->VolumeMultiplier,
				NextDetectedVolume,
				DeltaSecond,
				BgmFadeSpeed
			);

			BgmNormalComponent->SetVolumeMultiplier(lerpNormalVolume);
			BgmDetectedComponent->SetVolumeMultiplier(lerpDetectedVolume);

			if (BgmNormalComponent->VolumeMultiplier == NextNormalVolume ||
				BgmDetectedComponent->VolumeMultiplier == NextDetectedVolume)
			{
				ChangeVolume = false;
			}
		}
	}
}

