#pragma once

#include "CoreMinimal.h"
#include "AdInfo.generated.h"

USTRUCT(BlueprintType)
struct BIDAPP_API FAdInfo
{
    GENERATED_BODY()

    FString ToString() const;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString AdUnitPlacement;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString NetworkId;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString WaterfallId;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    double Revenue = 0;
};
