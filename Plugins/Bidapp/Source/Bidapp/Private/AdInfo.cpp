/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "AdInfo.h"


FString FAdInfo::ToString() const
{
    TArray<FStringFormatArg> Args;
    Args.Add(FStringFormatArg(AdUnitPlacement));
    Args.Add(FStringFormatArg(AdUnitIdentifier));
    Args.Add(FStringFormatArg(NetworkId));
    Args.Add(FStringFormatArg(WaterfallId));
    Args.Add(FStringFormatArg(Revenue));

    return FString::Format(TEXT("[FAdInfo adUnitPlacement: {0} adUnitIdentifier: {1} networkId: {2} waterfallId: {3} revenue: {4}]"), Args);
}
