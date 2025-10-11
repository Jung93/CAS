// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CAS_PatrolPathEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_PatrolPathEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
