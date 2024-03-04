#pragma once

#include "CoreMinimal.h"
#include "SdkConfiguration.generated.h"

// Enums

UENUM()
enum class EConsentFlowUserGeography : uint8
{
    Unknown,
    GDPR,
    Other
};

UENUM(BlueprintType)
enum class EAppTrackingStatus : uint8 // iOS only
{
    Unavailable,
    NotDetermined,
    Restricted,
    Denied,
    Authorized,
};

// MARK: - SdkConfiguration
USTRUCT(BlueprintType)
struct BIDAPP_API FSdkConfiguration
{
    GENERATED_BODY()

};
