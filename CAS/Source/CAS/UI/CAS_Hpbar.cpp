// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_Hpbar.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h" 
#include "Blueprint/WidgetTree.h"

void UCAS_Hpbar::UpdateHp(int32 count)
{
    CurHp += count;


}

void UCAS_Hpbar::NativeConstruct()
{
	Super::NativeConstruct();
	
    for (int32 i = 0; i < HpCount; ++i)
    {
        UWidgetTree* WidgetTreePtr = WidgetTree.Get();
        UImage* hpBlock = WidgetTreePtr->ConstructWidget<UImage>(HpBlock);
        if (hpBlock)
        {
            CAS_HpBar->AddChildToHorizontalBox(hpBlock);
        }
    }

}
