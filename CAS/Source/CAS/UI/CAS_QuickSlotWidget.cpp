// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidget.h"
#include "UI/CAS_SkillSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
void UCAS_QuickSlotWidget::InitSetting(int32 count)
{
	//SlotCount = count;
    //for (int32 i = 0; i < SlotCount; i++)
    //{
    //    UWidgetTree* WidgetTreePtr = WidgetTree.Get();
    //    UImage* hpBlock = WidgetTreePtr->ConstructWidget<UImage>(HpBlock);
    //    if (hpBlock)
    //    {
    //        CAS_HpBar->AddChildToHorizontalBox(hpBlock);
    //    }
    //}
    //SlotWidgets.SetNum(4);
    //SlotDataArray.SetNum(4);
    //
    //for (int32 i = 0; i < 4; ++i)
    //{
    //    if (SlotWidgetClass)
    //    {
    //        UQuickSlotWidget* NewSlot = CreateWidget<UQuickSlotWidget>(this, SlotWidgetClass);
    //        FQuickSlotData Data;
    //        Data.SlotIndex = i;
    //        Data.IconTexture = nullptr; // 초기값
    //        Data.AbilityTag = FName(TEXT("None"));
    //
    //        NewSlot->SetSlotData(Data);
    //        SlotWidgets[i] = NewSlot;
    //        SlotDataArray[i] = Data;
    //
    //        if (SlotBox)
    //        {
    //            SlotBox->AddChildToHorizontalBox(NewSlot);
    //        }
    //    }
    //}

}

void UCAS_QuickSlotWidget::SwapSlots(UCAS_SkillSlot* DragSlot, UCAS_SkillSlot* DropSlot)
{
    int32 DragIndex = DragSlot->GetSlotIndex();
    int32 DropIndex = DropSlot->GetSlotIndex();

    // Swap SlotData
    FCAS_SlotData Temp = SlotData[DragIndex];
    SlotData[DragIndex] = SlotData[DropIndex];
    SlotData[DropIndex] = Temp;

    SlotData[DragIndex].SlotIndex = DragIndex;
    SlotData[DropIndex].SlotIndex = DropIndex;

    DragSlot->SetSlotData(SlotData[DragIndex]);
    DropSlot->SetSlotData(SlotData[DropIndex]);
}

const FCAS_SlotData& UCAS_QuickSlotWidget::GetSlotData(int32 SlotIndex) const
{
    return SlotData[SlotIndex];
}

void UCAS_QuickSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SlotWidgets.SetNum(SlotCount);
    SlotData.SetNum(SlotCount);

    for (int32 i = 0; i < SlotCount; ++i)
    {
        if (SlotWidgetClass)
        {
            UCAS_SkillSlot* NewSlot = CreateWidget<FCAS_SlotData>(this, SlotWidgetClass);
            FCAS_SlotData Data;
            Data.SlotIndex = i;
            Data.SlotTexture = nullptr; // 초기값
            Data.AbilityTag = FName(TEXT("None"));

            NewSlot->SetSlotData(Data);
            SlotWidgets[i] = NewSlot;
            SlotData[i] = Data;

            if (SlotBox)
            {
                SlotBox->AddChildToHorizontalBox(NewSlot);
            }
        }
    }
}
