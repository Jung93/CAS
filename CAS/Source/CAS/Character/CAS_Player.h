// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CAS_Character.h"
#include "CAS_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CAS_API ACAS_Player : public ACAS_Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACAS_Player();
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StealAbility(const FInputActionValue& Value);

	void Capture(const FInputActionValue& Value);

	void ShowMouse(const FInputActionValue& Value);
	void HideMouse(const FInputActionValue& Value);

	void QuickSlotFunction(const FInputActionValue& Value);
	void QuickSlotInputReleased(const FInputActionValue& Value);
	//void OpenSlot(const FInputActionValue& Value);
	//void CloseSlot(const FInputActionValue& Value);
	void ChangeSlot01(const FInputActionValue& Value);
	void ChangeSlot02(const FInputActionValue& Value);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CaptureAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShowMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickSlotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenSlotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeSlotAction01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeSlotAction02;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	class ACAS_Hat* GetHat() { return _hatSpawn; }

public:	
	virtual void InitAbilitySystemComponent() override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual class UCAS_AttributeSet* GetAttributeSet() const override;

	class UCAS_QuickSlotWidget* GetQuickSlotWidget() { return QuickSlotWidget; }
	class UCAS_SelectSkillWidget* GetSelectSkillWidget() { return SelectSkillWidget; }
	class UCAS_QuickSlotWidgetComponent* GetQuickSlotWidgetComponent() { return QuickSlotWidgetComponent; }
	void AddPlayerAbility(TSubclassOf<class UGameplayAbility> newAbility);
protected:
	
	UPROPERTY(EditAnywhere, Category = "Hat")
	TSubclassOf<class ACAS_Hat> _hatBP;

	UPROPERTY()
	class ACAS_Hat* _hatSpawn = nullptr;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerAblilties|QuickSlots")
	TSubclassOf<class UCAS_QuickSlotWidget> QuickSlotWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	int32 PlayerAbilityCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAblilties|SelectSKill")
	TSubclassOf<class UCAS_SelectSkillWidget> SelectSkillWidgetClass;


	class UCAS_QuickSlotWidget* QuickSlotWidget;
	class UCAS_QuickSlotWidgetComponent* QuickSlotWidgetComponent;
	class UCAS_SelectSkillWidget* SelectSkillWidget;

};
