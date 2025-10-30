// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager/CAS_SpotLight.h"
#include "Components/SpotLightComponent.h"
#include "LevelManager/CAS_WorldSubsystem.h"
#include "Global/CAS_GameInstance.h"

ACAS_SpotLight::ACAS_SpotLight()
{

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	RootComponent = SpotLight;

	if (SpotLight)
	{
		SpotLight->Intensity = 10000.0f; // 밝기
		SpotLight->IntensityUnits = ELightUnits::Unitless;
		SpotLight->LightColor = FColor(255, 255, 255, 1); // 색상
		SpotLight->OuterConeAngle = 44.0f; // 외부 원뿔 각도
		SpotLight->AttenuationRadius = 1000.0f; // 감쇠 반경
		SpotLight->SetMobility(EComponentMobility::Stationary); // 이동성 설정
		SpotLight->SetRelativeRotation(FRotator(-90, 0, 0));
		SetActorHiddenInGame(true);
	}

}

void ACAS_SpotLight::BeginPlay()
{
	Super::BeginPlay();
	

	auto PuzzleSubsystem = GetWorld()->GetSubsystem<UCAS_WorldSubsystem>();
	PuzzleSubsystem->OnPuzzleCompleted.AddUObject(this, &ThisClass::LightOn);

	auto gi = Cast<UCAS_GameInstance>(GetGameInstance());

	if (gi && gi->IsPuzzleClear())
	{
		LightOn();
	}
}

void ACAS_SpotLight::LightOn()
{
	SetActorHiddenInGame(false);
}


