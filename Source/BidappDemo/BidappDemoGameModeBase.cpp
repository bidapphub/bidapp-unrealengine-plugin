#include "BidappDemoGameModeBase.h"
#include "Bidapp.h"
#include "DemoLogger.h"

ABidappDemoGameModeBase::ABidappDemoGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    UBidapp::SdkInitializedDelegate.AddLambda([this](const FSdkConfiguration& SdkConfiguration)
    {
        DEMO_LOG("Bidapp SDK Initialized");
    });

}
