#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDescriptionData.generated.h"

USTRUCT(BlueprintType)
struct FSkillDescriptionData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
};