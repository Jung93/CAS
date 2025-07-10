// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/CAS_GameplayAbility.h"
#include "CAS_SkillSlot.generated.h"

UCLASS()
class CAS_API UCAS_SkillSlot : public UUserWidget
{
	GENERATED_BODY()
public:
    void SetSlotData(const FCAS_SkillData& InitData);
    const FCAS_SkillData& GetSlotData() const { return DragSlotData; }
    int32 GetSlotIndex() const { return DragSlotData.SlotIndex; }

protected:
    void UpdateIcon();

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
    class UImage* CAS_Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTexture2D* DefaultTexture;

    UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
    TSubclassOf<UCAS_SkillSlot> SkillSlotWidgetClass;
protected:
    FCAS_SkillData DragSlotData;
};
