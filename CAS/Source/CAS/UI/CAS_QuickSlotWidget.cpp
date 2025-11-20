// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidget.h"
#include "Controller/CAS_PlayerController.h"
#include "UI/CAS_SkillSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Data/InputKeyIconData.h"
#include "Containers/Array.h"


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

            SkillSlots[i] = slot;
            if (rootBorder->IsValidLowLevel())
            {
                auto box = Cast<UHorizontalBox>(rootBorder->GetContent());
                box->AddChildToHorizontalBox(slot);
            }
        }
    }

    auto controller = Cast<ACAS_PlayerController>(GetOwningPlayer());

    if (controller->IsValidLowLevel())
    {
        controller->OnInputDeviceChanged.AddUObject(this, &UCAS_QuickSlotWidget::ChangeInputDeviceUI);
        controller->ChageQuickSlotTexture.AddUObject(this, &UCAS_QuickSlotWidget::ChangeSlotTexture);

        TArray<TPair<FName, FKeyMappingRow>> array = controller->GetUserSetting()->GetCurrentKeyProfile()->GetPlayerMappingRows().Array();

        for (int32 i = 1; i < 3; i++)
        {
            FString TargetStr = FString::Printf(TEXT("SlotChange%d"), i);
            FName TargetName = FName(TargetStr);

            TPair<FName, FKeyMappingRow>* KeyPair = array.FindByPredicate([TargetName](const TPair<FName, FKeyMappingRow>& Pair)
                {
                    return Pair.Key.IsEqual(TargetName);
                });

            if (!KeyPair)
                return;

            TArray<FPlayerKeyMapping> KeyMap = KeyPair->Value.Mappings.Array();

            FName KeyName = KeyMap[0].GetCurrentKey().GetFName();

            UTexture2D* Icon = nullptr;

            const FInputKeyIconData* Row = KeyIconTable->FindRow<FInputKeyIconData>(KeyName, "Jump");
            if (Row)
            {
                Icon = Row->Icon.LoadSynchronous();
            }

            if (TargetName.IsEqual("SlotChange1"))
            {
                Keyboard_Left->SetBrushFromTexture(Icon);
            }
            else if (TargetName.IsEqual("SlotChange2"))
            {
                Keyboard_Right->SetBrushFromTexture(Icon);

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

void UCAS_QuickSlotWidget::SwitchToggle()
{
    //UCanvasPanel* root = Cast<UCanvasPanel>(GetRootWidget());
    //UCanvasPanel* panel = Cast<UCanvasPanel>(root->GetChildAt(0));
    //UCanvasPanel* imagePanel = Cast<UCanvasPanel>(panel->GetChildAt(4));
    //UBorder* toggleBorder = Cast<UBorder>(imagePanel->GetChildAt(2));

    isToggled = !isToggled;

    if (isToggled)
    {
        ToggleEffectBorder->SetBrushFromMaterial(ToggleBorderMaterial);
        ToggleEffectBorder->SetRenderOpacity(1);
    }
    else
    {
        ToggleEffectBorder->SetBrushFromMaterial(nullptr);
        ToggleEffectBorder->SetRenderOpacity(0);
    }
}

void UCAS_QuickSlotWidget::ChangeInputDeviceUI(EInputDeviceType InputDevice)
{
    if (InputDevice == EInputDeviceType::KeyboardMouse)
    {
        Keyboard_Left->SetColorAndOpacity(FLinearColor(1,1,1,1));
        Keyboard_Right->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
        Controller_Left->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
        Controller_Right->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
    }
    else 
    {
        Keyboard_Left->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
        Keyboard_Right->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
        Controller_Left->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
        Controller_Right->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
    }

}

void UCAS_QuickSlotWidget::ChangeSlotTexture(FName ActionName, UTexture2D* Texture)
{
    if (ActionName.IsEqual("SlotChange1"))
    {
        Keyboard_Left->SetBrushFromTexture(Texture);
    }
    else if (ActionName.IsEqual("SlotChange2"))
    {
        Keyboard_Right->SetBrushFromTexture(Texture);
    }

}
