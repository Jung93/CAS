// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/CAS_AttributeSet.h"
#include "GAS/CAS_GamePlayTag.h"
#include "GAS/CAS_AbilityInputID.h"
#include "AbilitySystemInterface.h"
#include "GAS/CAS_AbilitySystemComponent.h"
#include "AI/CAS_PatrolPath.h"
#include "Controller/CAS_EnemyController.h"
#include "CAS_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EMoveType : uint8
{
	Idle, Patrol, Walk, Sprint , PreAttackPhase
};

UCLASS(Abstract)
class CAS_API ACAS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACAS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void DeadEvent();
	virtual void TakeDamageEvent();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const;
	virtual UCAS_AttributeSet* GetAttributeSet() const	{PURE_VIRTUAL(ACAS_Character::GetAttributeSet, return nullptr;);}

	virtual void AddDefaultAbilites();
	virtual void InitAbilitySystemComponent();
	void SetHp(int32 value);
	void SetWalkSpeed(int32 value);

	void SetJumpForce(float value);
	void OnJumpForceChanged(const FOnAttributeChangeData& Data);
	void OnWalkSpeedChanged(const FOnAttributeChangeData& Data);

	virtual void ActivateAbility(const FGameplayTag tag);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UCAS_GameplayAbility*>& Abilities);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool CanActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);


	void SetAudioComponent(class UAudioComponent* NewComponent) { AuidoComponent = NewComponent; };
	class UAudioComponent* GetAudioComponent() { return AuidoComponent; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UCAS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UCAS_AttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
	class UAudioComponent* AuidoComponent;


public:
	EEnemyType GetEnemyType() { return MyType; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyType")
	EEnemyType MyType = EEnemyType::Nomal;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HpBar")
	class UWidgetComponent* HpBarWidgetComponent;
	UPROPERTY(EditAnywhere, Category = "HpBar")
	TSubclassOf<class UUserWidget> HpBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = "HpBar")
	int32 HpCount = 3;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACAS_PatrolPath* PatrolPath;
public:
	ACAS_PatrolPath* GetPatrolPath() { return PatrolPath; }

public:
	virtual void SaveCharacterData();
	virtual void LoadCharacterData();


};

