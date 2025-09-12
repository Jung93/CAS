// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_KeyPressUI.h"
#include "Components/Image.h"

void UCAS_KeyPressUI::SetTexture(UTexture2D* Texture)
{
	CAS_KeyImage->SetBrushFromTexture(Texture);
}
