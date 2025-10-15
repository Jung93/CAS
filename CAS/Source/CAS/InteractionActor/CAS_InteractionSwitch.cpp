// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor/CAS_InteractionSwitch.h"

void ACAS_InteractionSwitch::BeginPlay()
{
    Super::BeginPlay();

    auto Material = StaticMesh->GetMaterial(0);
    
    if (Material)
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

        StaticMesh->SetMaterial(0, DynamicMaterial);

        DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));

    }

}
void ACAS_InteractionSwitch::InteractionWithPlayer()
{
    if (bSwitchOn) {
        DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.8f, 0.3f, 0.0f, 1.0f));
    }
    else {
        DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.6f, 0.0f, 1.0f));
    }

    bSwitchOn = !bSwitchOn;

    OnSwitchClicked.Broadcast(bSwitchOn);
}

