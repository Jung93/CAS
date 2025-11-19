// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"	
#include "Components/Image.h"
#include "CAS_MouseSelectionWidget.generated.h"


UENUM(BlueprintType)
enum class EWidgetPositionType : uint8
{
	NONE,Left,Right
};

DECLARE_MULTICAST_DELEGATE_OneParam(FWidgetClickEvent_child, EWidgetPositionType);

UCLASS()
class CAS_API UCAS_MouseSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	EWidgetPositionType GetWidgetPositionType() { return WidgetPositionType; }
	void SetWidgetPositionType(EWidgetPositionType PositionType) { WidgetPositionType = PositionType; }
	void SetTexture(class UTexture2D* Texture);

	FWidgetClickEvent_child WidgetClickEvent_child;

protected:
	UFUNCTION()
	void ClickedEvent();
	UFUNCTION()
	void HoveredEvent();
	UFUNCTION()
	void UnhoveredEvent();
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* CAS_IconImage;
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* CAS_Button;
	UPROPERTY(EditAnywhere)
	EWidgetPositionType WidgetPositionType = EWidgetPositionType::NONE;

	FLinearColor EnterColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FLinearColor LeaveColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.9f);
};
