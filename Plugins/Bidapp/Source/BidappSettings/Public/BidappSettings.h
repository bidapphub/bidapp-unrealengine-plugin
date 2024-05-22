/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "BidappSettings.generated.h"



class FToolBarBuilder;
class FMenuBuilder;
UCLASS(transient, config = Engine)
class BIDAPPSETTINGS_API UBidappSettings : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(config, EditAnywhere, Category = "Bidapp PubId")
    FString PubId;

    UPROPERTY(config, EditAnywhere, Category = "Bidapp SDK settings")
    bool bEnableTestMode;

    UPROPERTY(config, EditAnywhere, Category = "Bidapp SDK settings")
    bool bEnableLogging;


    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bLiftoff;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bApplovin;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bApplovinMax;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bUnity;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bAdmob;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bChartboost;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bDigitalTurbine;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bFacebook;

    UPROPERTY(config, EditAnywhere, Category = "Networks Dependencies")
    bool bYandex;

    UPROPERTY(config, EditAnywhere, Category = "Admob Application Id")
    FString AdmobAppId;

    UBidappSettings()
        : PubId(TEXT("")), bEnableTestMode(false), bEnableLogging(true), 
          bLiftoff(true), bApplovin(true),
          bApplovinMax(true), bUnity(true),
          bAdmob(true), bChartboost(true),
          bDigitalTurbine(true), bFacebook(true),
          bYandex(true), AdmobAppId(TEXT(""))
    {
    }

    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};


class FBidappSettings : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    void SetupAARImports();
};

