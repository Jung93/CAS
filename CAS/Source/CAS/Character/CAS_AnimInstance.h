// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:	
	void CheckControllerClass(AController* Controller);
	
	bool GetAttackMode() { return bAttackMode; }
	void SetAttackMode(bool IsAttackMode) { bAttackMode = IsAttackMode; }
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bAttackMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bPlayerControl = false;
};
