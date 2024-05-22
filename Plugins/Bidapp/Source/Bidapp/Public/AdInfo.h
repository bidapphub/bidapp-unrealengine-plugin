/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/



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
    FString AdUnitIdentifier;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString NetworkId;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString WaterfallId;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    double Revenue = 0;
};
