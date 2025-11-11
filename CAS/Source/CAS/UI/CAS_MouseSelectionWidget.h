// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
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
	EWidgetPositionType GetWidgetPositionType() { return WidgetPositionType; }
	void SetWidgetPositionType(EWidgetPositionType PositionType) { WidgetPositionType = PositionType; }
	
	FWidgetClickEvent_child WidgetClickEvent_child;
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* CAS_Border;

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* CAS_IconImage;

	UPROPERTY(EditAnywhere)
	EWidgetPositionType WidgetPositionType = EWidgetPositionType::NONE;

	FLinearColor EnterColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FLinearColor LeaveColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.7f);
};
