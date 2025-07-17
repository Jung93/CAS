// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidget.h"
#include "UI/CAS_SkillSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"

void UCAS_QuickSlotWidget::InitSetting(int32 count)
{
    SlotCount = count;

    SkillSlots.SetNum(SlotCount);



    for (int32 i = 0; i < SlotCount;i++)
    {
        UCanvasPanel* root = Cast<UCanvasPanel>(GetRootWidget());

        UCanvasPanel* panel = Cast<UCanvasPanel>(root->GetChildAt(0));

        UBorder* rootBorder = Cast<UBorder>(panel->GetChildAt(i));



        if (SlotWidgetClass)
        {
            UCAS_SkillSlot* slot = CreateWidget<UCAS_SkillSlot>(GetWorld(), SlotWidgetClass);
            FCAS_SlotData Data;
            Data.SlotIndex = i;
            Data.SkillData.AbilityIconTexture = nullptr;
            Data.SkillData.AbilityTag = FName(TEXT("None"));
            slot->SetSlotData(Data);

            if (i != 0)
            {
                FLinearColor initColor = slot->GetColorAndOpacity();
                initColor.A = 0.3f;
                slot->SetColorAndOpacity(initColor);
            }
            //else
            //{
            //    auto slotBorder = Cast<UBorder>(slot->GetRootWidget());
            //    auto imageBox = Cast<UImage>(slotBorder->GetContent());

            //    auto panelBorder = Cast<UBorder>(SlotBox->GetParent());
            //    auto panelSlot = Cast<UCanvasPanelSlot>(panelBorder->Slot);
            //  
            //    FVector2D Size = imageBox->Brush.GetImageSize();
            //    Size.X += slotBorder->Padding.Right + 2.0f;
            //    panelSlot->SetSize(Size);

            //    FVector2D Pos = panelSlot->GetPosition();
            //    Pos.X = 200.f;
            //    panelSlot->SetPosition(Pos);

            //    BorderSizeX = Size.X;
            //    BorderPosX = Pos.X;

            //}

            SkillSlots[i] = slot;
            if (rootBorder->IsValidLowLevel())
            {
                auto box = Cast<UHorizontalBox>(rootBorder->GetContent());
                box->AddChildToHorizontalBox(slot);


                //SlotBox->AddChildToHorizontalBox(slot);
            }
        }
    }
}

void UCAS_QuickSlotWidget::SwapSlots(UCAS_SkillSlot* DragSlot, UCAS_SkillSlot* DropSlot)
{
    int32 DragIndex = DragSlot->GetSlotIndex();
    int32 DropIndex = DropSlot->GetSlotIndex();

    if (DragIndex == DropIndex) {
        return;
    }

    if (DropIndex == 4)
        return;

    if (DragIndex == 4)
    {
        FCAS_SlotData DragSlotData = DragSlot->GetSlotData();
        FCAS_SlotData DropSlotData = DropSlot->GetSlotData();

        DragSlotData.SlotIndex = DropIndex;

        DropSlot->SetSlotData(DragSlotData);

        return;
    }

    FCAS_SlotData DragSlotData = DragSlot->GetSlotData();
    FCAS_SlotData DropSlotData = DropSlot->GetSlotData();

    DragSlotData.SlotIndex = DropIndex;
    DropSlotData.SlotIndex = DragIndex;

    DragSlot->SetSlotData(DropSlotData);
    DropSlot->SetSlotData(DragSlotData);

    QuickSlotSwapEvent.Broadcast(DragIndex, DropSlotData);
    QuickSlotSwapEvent.Broadcast(DropIndex, DragSlotData);

}

void UCAS_QuickSlotWidget::SetSlotData(int32 index, const FCAS_SlotData& AbilityData)
{
    SkillSlots[index]->SetSlotData(AbilityData);
}

void UCAS_QuickSlotWidget::RemoveSlotData(int32 index)
{
    FCAS_SlotData Data;
    Data.SlotIndex = index;
    Data.SkillData.AbilityIconTexture = nullptr;
    Data.SkillData.AbilityTag = FName(TEXT("None"));
   
    SkillSlots[index]->SetSlotData(Data);
}

void UCAS_QuickSlotWidget::OpenSlot()
{
    UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget());

    if (RootCanvas->IsValidLowLevel())
    {
        UBorder* Border = Cast<UBorder>(RootCanvas->GetChildAt(0));

        if (Border->IsValidLowLevel())
        {
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border->Slot);

            if (CanvasSlot->IsValidLowLevel())
            {
                // 위치 변경
                FVector2D Pos = CanvasSlot->GetPosition();
                Pos.X = 0.0f;
                CanvasSlot->SetPosition(Pos);

                // 크기 변경
                FVector2D Size = CanvasSlot->GetSize();
                Size.X = 550.f;
                CanvasSlot->SetSize(Size);

                //for (int i = 1; i < SlotCount; i++)
                //{
                //    auto slot = Cast<UCAS_SkillSlot>(SlotBox->GetChildAt(i));

                //    FLinearColor initColor = slot->GetColorAndOpacity();
                //    initColor.A = 0.3f;
                //    slot->SetColorAndOpacity(initColor);

                //}


            }
        }
    }
}

void UCAS_QuickSlotWidget::CloseSlot()
{
    UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget());

    if (RootCanvas->IsValidLowLevel())
    {
        UBorder* Border = Cast<UBorder>(RootCanvas->GetChildAt(0));

        if (Border->IsValidLowLevel())
        {
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border->Slot);

            if (CanvasSlot->IsValidLowLevel())
            {
                // 위치 변경
                FVector2D Pos = CanvasSlot->GetPosition();
                Pos.X = BorderPosX;
                CanvasSlot->SetPosition(Pos);

                // 크기 변경
                FVector2D Size = CanvasSlot->GetSize();
                Size.X = BorderSizeX;
                CanvasSlot->SetSize(Size);

                //for (int i = 1; i < SlotCount; i++)
                //{
                //    auto slot = Cast<UCAS_SkillSlot>(SlotBox->GetChildAt(i));

                //    FLinearColor initColor = slot->GetColorAndOpacity();
                //    initColor.A = 0.0f;
                //    slot->SetColorAndOpacity(initColor);

                //}
            }
        }
    }
}



void UCAS_QuickSlotWidget::ChangeSlotToLeft()
{

    int32 num = SkillSlots.Num() - 1;
    UCAS_SkillSlot* slot = SkillSlots[0];
    FCAS_SlotData data = slot->GetSlotData();
    data.SlotIndex = SkillSlots[num]->GetSlotIndex();

    for (int32 i = 0; i < num; i++)
    {
        FCAS_SlotData targetData = SkillSlots[i + 1]->GetSlotData();
        targetData.SlotIndex = SkillSlots[i]->GetSlotIndex();

        SkillSlots[i]->SetSlotData(targetData);
    }

    SkillSlots[num]->SetSlotData(data);
}


void UCAS_QuickSlotWidget::ChangeSlotToRight()
{

    int32 num = SkillSlots.Num() - 1;
    UCAS_SkillSlot* slot = SkillSlots[num];
    FCAS_SlotData data = slot->GetSlotData();
    data.SlotIndex = SkillSlots[0]->GetSlotIndex();

    for (int32 i = num; i > 0; i--)
    {
        FCAS_SlotData targetData = SkillSlots[i - 1]->GetSlotData();
        targetData.SlotIndex = SkillSlots[i]->GetSlotIndex();

        SkillSlots[i]->SetSlotData(targetData);
    }

    SkillSlots[0]->SetSlotData(data);
}

void UCAS_QuickSlotWidget::BlockSlotSwap(TArray<UCAS_SkillSlot*> SelectSkillSlots)
{
    for (int32 i = 0; i < SlotCount;i++)
    {
        SelectSkillSlots[i]->SwitchDragable();
    }

}
