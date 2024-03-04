#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBidapp, Log, All);

DEFINE_LOG_CATEGORY(LogBidapp);

#define MAX_D(message, ...)           UE_LOG(LogBidapp, Display, TEXT(message), ##__VA_ARGS__)
#define MAX_W(message, ...)           UE_LOG(LogBidapp, Warning, TEXT(message), ##__VA_ARGS__)
#define MAX_E(message, ...)           UE_LOG(LogBidapp, Error, TEXT(message), ##__VA_ARGS__)

#define MAX_USER_DEBUG(message, ...)      { if (UBidapp::IsVerboseLoggingEnabled()) { UE_LOG(LogBidapp, Display, TEXT(message), ##__VA_ARGS__) }}
#define MAX_USER_WARN(message, ...)    { if (UBidapp::IsVerboseLoggingEnabled()) { UE_LOG(LogBidapp, Warning, TEXT(message), ##__VA_ARGS__) }}
#define MAX_USER_ERROR(message, ...)      { if (UBidapp::IsVerboseLoggingEnabled()) { UE_LOG(LogBidapp, Error, TEXT(message), ##__VA_ARGS__) }}
