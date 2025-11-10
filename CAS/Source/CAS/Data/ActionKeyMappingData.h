#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"    // FTableRowBase 사용을 위해 필요
#include "ActionKeyMappingData.generated.h"

USTRUCT(BlueprintType)
struct FActionKeyMappingData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName KeyName;
};