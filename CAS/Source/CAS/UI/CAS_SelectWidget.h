// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CAS_SelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_SelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//실행시킬 슬롯의 인덱스 , 버튼 두개 
public:
	void SetSellectedIndex(int32 index) { SellectedIndex = index; }
	
	void YES_OnClickedEvent(UObject* Object, FName name);
	void NO_OnClickedEvent(UObject* Object, FName name);
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* YES_Button;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* NO_Button;

	int32 SellectedIndex = -1;
};
