// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SelectSkillWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Character//CAS_Player.h"
#include "UI/CAS_SkillSlot.h"
#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "GAS/CAS_GameplayAbility.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UCAS_SelectSkillWidget::InitSetting()
{
    SkillSlots.SetNum(5);

	auto root = Cast<UCanvasPanel>(GetRootWidget());

	auto border = Cast<UBorder>(root->GetChildAt(0));

	auto slot2 = Cast<UCanvasPanelSlot>(border->Slot);

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	ViewportSize /= Scale; // DPI 보정 적용

	slot2->SetSize(ViewportSize);


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
            Data.SlotTexture = nullptr;
            Data.AbilityTag = FName(TEXT("None"));

            slot->SetSlotData(Data);


            SkillSlots[i] = slot;
            if (hbox->IsValidLowLevel())
            {
                hbox->AddChildToHorizontalBox(slot);

            }
        }

        //테스트용 코드
        FString a = "aaa" + FString::FromInt(i + 1);
        FText aaa = FText::FromString(a);

        text->SetText(aaa);


	}


}

void UCAS_SelectSkillWidget::SetSlots(const TArray<UCAS_SkillSlot*> CurrentSkillSlots, const TSubclassOf<class UGameplayAbility>& newAbility)
{

    //SkillSlots에 있는 정보를 바탕으로 SelectSkillWidget의 정보를 덧씌우기

    for (int32 i = 0; i < CurrentSkillSlots.Num(); i++)
    {
        SkillSlots[i]->SetSlotData(CurrentSkillSlots[i]->GetSlotData());
    }


    //새 어빌리티는 마지막 인덱스에 넣기
    auto DefaultObj = newAbility->GetDefaultObject<UCAS_GameplayAbility>();

    if (DefaultObj->IsValidLowLevel()) {
        UTexture2D* TextureData = DefaultObj->AbilityIcon;
        FName TagData = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();

        FCAS_SlotData Data;
        Data.SlotIndex = SkillSlots.Num() - 1;

        Data.SlotTexture = TextureData;
        Data.AbilityTag = TagData;

        SkillSlots[SkillSlots.Num() - 1]->SetSlotData(Data);

        TargetAbility = newAbility;
    }

}

void UCAS_SelectSkillWidget::SetSlots(int32 TargetIndex, TArray<UCAS_SkillSlot*> TargetSkillSlots)
{

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
