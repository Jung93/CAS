// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAS_KeyPressUI.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_KeyPressUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetTexture(class UTexture2D* Texture);
protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CAS_KeyImage;
};
