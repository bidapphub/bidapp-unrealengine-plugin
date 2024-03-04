#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BidappDemoGameModeBase.generated.h"

struct FSdkConfiguration;


UCLASS()
class BIDAPPDEMO_API ABidappDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABidappDemoGameModeBase(const FObjectInitializer& ObjectInitializer);
};
