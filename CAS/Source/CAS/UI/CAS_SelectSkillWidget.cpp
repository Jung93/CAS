// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SelectSkillWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Character//CAS_Player.h"
#include "UI/CAS_SkillSlot.h"
#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "GAS/CAS_GameplayAbility.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Data/SkillDescriptionData.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"



void UCAS_SelectSkillWidget::InitSetting()
{
    SkillSlots.SetNum(5);

	auto root = Cast<UCanvasPanel>(GetRootWidget());
	auto border = Cast<UBorder>(root->GetChildAt(0));

	auto borderPanel = Cast<UCanvasPanel>(border->GetContent());

	for (int32 i = 0; i < 5; i++)
	{
		auto panel = Cast<UCanvasPanel>(borderPanel->GetChildAt(i));
		auto hbox = Cast<UHorizontalBox>(panel->GetChildAt(0));
		auto text = Cast<UTextBlock>(panel->GetChildAt(1));

        if (SlotWidgetClass)
        {
            UCAS_SkillSlot* slot = CreateWidget<UCAS_SkillSlot>(GetWorld(), SlotWidgetClass);
            FCAS_SlotData Data;
            Data.SlotIndex = i;
            Data.SkillData.AbilityIconTexture = nullptr;
            Data.SkillData.AbilityTag = FName(TEXT("None"));

            slot->SetSlotData(Data);

            SkillSlots[i] = slot;
            if (hbox->IsValidLowLevel())
            {
                hbox->AddChildToHorizontalBox(slot);
            }
            TextBlocks.Add(text);
        }
	}
}

void UCAS_SelectSkillWidget::SetSlots(const TArray<UCAS_SkillSlot*> CurrentSkillSlots, const TSubclassOf<class UGameplayAbility>& newAbility)
{
    for (int32 i = 0; i < CurrentSkillSlots.Num(); i++)
    {
        SkillSlots[i]->SetSlotData(CurrentSkillSlots[i]->GetSlotData());

        auto text = TextBlocks[i];

        FName AbilityTagName = SkillSlots[i]->GetSlotData().SkillData.AbilityTag;
        FText AbilityDesc = FText();

        const FSkillDescriptionData* Row = DescriptionTable->FindRow<FSkillDescriptionData>(AbilityTagName, "");

        if (Row)
        {
            AbilityDesc = Row->Description;
        }

        text->SetText(AbilityDesc);
    }

    auto DefaultObj = newAbility->GetDefaultObject<UCAS_GameplayAbility>();

    if (DefaultObj->IsValidLowLevel()) {
        FCAS_SkillData SkillData = DefaultObj->GetSkillData();
        FName TagData = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();

        FCAS_SlotData Data;
        Data.SlotIndex = SkillSlots.Num() - 1;

        Data.SkillData = SkillData;

        SkillSlots[SkillSlots.Num() - 1]->SetSlotData(Data);

        FName AbilityTagName = Data.SkillData.AbilityTag;
        FText TagName = FText();

        const FSkillDescriptionData* Row = DescriptionTable->FindRow<FSkillDescriptionData>(AbilityTagName, "");

        if (Row)
        {
            TagName = Row->Description;
        }

        TextBlocks[SkillSlots.Num() - 1]->SetText(TagName);
        TargetAbility = newAbility;
    }

    AddToViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true;
    PC->bEnableMouseOverEvents = true;
}

void UCAS_SelectSkillWidget::SetSlots(int32 TargetIndex, TArray<UCAS_SkillSlot*> TargetSkillSlots)
{

    UGameplayStatics::SetGamePaused(GetWorld(), false);
    RemoveFromParent();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    PC->bShowMouseCursor = false;
    PC->bEnableClickEvents = false;
    PC->bEnableMouseOverEvents = false;

    for (int32 i = 0; i < TargetSkillSlots.Num();i++)
    {
        TargetSkillSlots[i]->SetSlotData(SkillSlots[i]->GetSlotData());
    }

    auto player = Cast<ACAS_Player>(GetOwningPlayerPawn());

    if (player->IsValidLowLevel())
    {
        auto quickSlotWidgetComponent = player->GetQuickSlotWidgetComponent();

        if (quickSlotWidgetComponent->IsValidLowLevel())
        {
            quickSlotWidgetComponent->RemovePlayerAbility(TargetIndex);
            quickSlotWidgetComponent->AddPlayerAbility(TargetIndex, TargetAbility);
        }
    }

    TargetAbility = nullptr;

}