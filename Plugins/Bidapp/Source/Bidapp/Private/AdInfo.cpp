#include "AdInfo.h"


FString FAdInfo::ToString() const
{
    TArray<FStringFormatArg> Args;
    Args.Add(FStringFormatArg(AdUnitPlacement));
    Args.Add(FStringFormatArg(NetworkId));
    Args.Add(FStringFormatArg(WaterfallId));
    Args.Add(FStringFormatArg(Revenue));

    return FString::Format(TEXT("[FAdInfo adUnitPlacement: {0} networkId: {1} waterfallId: {2} revenue: {3}]"), Args);
}
