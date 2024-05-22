/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#pragma once

#include "CoreMinimal.h"
#include "AdError.generated.h"

USTRUCT(BlueprintType)
struct BIDAPP_API FAdError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Bidapp")
    FString AdUnitIdentifier;
};
