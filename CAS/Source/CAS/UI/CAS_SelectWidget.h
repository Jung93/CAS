// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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
	void SetSelectedIndex(int32 index) { SelectedIndex = index; }
	int32 GetSelectedIndex() { return SelectedIndex; }

	void SetWidgetText(FText text);
	void SetWidgetText(FString string);

	void YES_OnClickedEvent(UObject* Object, FName name);
	void NO_OnClickedEvent(UObject* Object, FName name);
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* YES_Button;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* NO_Button;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CAS_TextBlock;

	int32 SelectedIndex = -1;
};
