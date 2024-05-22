/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJava.h"

class FJavaAndroidBidappUnrealPlugin : public FJavaClassObject
{
public:
    FJavaAndroidBidappUnrealPlugin();
    virtual ~FJavaAndroidBidappUnrealPlugin();

    // MARK: Initialization
    void Initialize(const FString &PluginVersion, const FString &SdkKey);
    bool IsInitialized();

    // MARK: Privacy
    void SetGDPR(bool bGDPR);
    void SetCOPPA(bool bCOPPA);
    void SetCCPA(bool bCCPA);

    // MARK: General
    void SetVerboseLoggingEnabled();
    bool IsVerboseLoggingEnabled();
    void SetTestEnable();


    // MARK: Banners
    void CreateBanner(const FString &AdUnitIdentifier, const FString &BannerVericalPosition, const FString &BannerHorizontalPosition);
    void ShowBanner(const FString &AdUnitIdentifier);
    void HideBanner(const FString &AdUnitIdentifier);
    void DestroyBanner(const FString &AdUnitIdentifier);
    void StartAutorefreshBanner(const FString &AdUnitIdentifier, int32 interval);
    void StopAutorefreshBanner(const FString &AdUnitIdentifier);

    
    // MARK: Mrec
    void CreateMrec(const FString &AdUnitIdentifier, const FString &MrecVericalPosition, const FString &MrecHorizontalPosition);
    void ShowMrec(const FString &AdUnitIdentifier);
    void HideMrec(const FString &AdUnitIdentifier);
    void DestroyMrec(const FString &AdUnitIdentifier);
    void StartAutorefreshMrec(const FString &AdUnitIdentifier, int32 interval);
    void StopAutorefreshMrec(const FString &AdUnitIdentifier);

     // MARK: Leaderboard
    void CreateLeaderboard(const FString &AdUnitIdentifier, const FString &LeaderboardVericalPosition, const FString &LeaderboardHorizontalPosition);
    void ShowLeaderboard(const FString &AdUnitIdentifier);
    void HideLeaderboard(const FString &AdUnitIdentifier);
    void DestroyLeaderboard(const FString &AdUnitIdentifier);
    void StartAutorefreshLeaderboard(const FString &AdUnitIdentifier, int32 interval);
    void StopAutorefreshLeaderboard(const FString &AdUnitIdentifier);

    // MARK: Interstitials
    void CreateInterstitial(const FString &AdUnitIdentifier, bool autoCaching);
    bool IsInterstitialReady(const FString &AdUnitIdentifier);
    void ShowInterstitial(const FString &AdUnitIdentifier);
    void LoadInterstitial(const FString &AdUnitIdentifier);
    void DestroyInterstitial(const FString &AdUnitIdentifier);

    // MARK: Rewarded
    void CreateRewarded(const FString &AdUnitIdentifier, bool autoCaching);
    bool IsRewardedReady(const FString &AdUnitIdentifier);
    void ShowRewarded(const FString &AdUnitIdentifier);
    void LoadRewarded(const FString &AdUnitIdentifier);
    void DestroyRewarded(const FString &AdUnitIdentifier);

private:
    static FName GetClassName();

    FJavaClassMethod InitializeMethod;
    FJavaClassMethod IsInitializedMethod;

    FJavaClassMethod SetGDPRMethod;
    FJavaClassMethod SetCOPPAMethod;
    FJavaClassMethod SetCCPAMethod;

    FJavaClassMethod SetVerboseLoggingEnabledMethod;
    FJavaClassMethod IsVerboseLoggingEnabledMethod;
    FJavaClassMethod SetTestEnableMethod;
 
    FJavaClassMethod CreateBannerMethod;
    FJavaClassMethod ShowBannerMethod;
    FJavaClassMethod HideBannerMethod;
    FJavaClassMethod DestroyBannerMethod;
    FJavaClassMethod StartAutorefreshBannerMethod;
    FJavaClassMethod StopAutorefreshBannerMethod;

    FJavaClassMethod CreateMrecMethod;
    FJavaClassMethod ShowMrecMethod;
    FJavaClassMethod HideMrecMethod;
    FJavaClassMethod DestroyMrecMethod;
    FJavaClassMethod StartAutorefreshMrecMethod;
    FJavaClassMethod StopAutorefreshMrecMethod;

    FJavaClassMethod CreateLeaderboardMethod;
    FJavaClassMethod ShowLeaderboardMethod;
    FJavaClassMethod HideLeaderboardMethod;
    FJavaClassMethod DestroyLeaderboardMethod;
    FJavaClassMethod StartAutorefreshLeaderboardMethod;
    FJavaClassMethod StopAutorefreshLeaderboardMethod;

    FJavaClassMethod IsInterstitialReadyMethod;
    FJavaClassMethod ShowInterstitialMethod;
    FJavaClassMethod CreateInterstitialMethod;
    FJavaClassMethod LoadInterstitialMethod;
    FJavaClassMethod DestroyInterstitialMethod;

    FJavaClassMethod IsRewardedReadyMethod;
    FJavaClassMethod ShowRewardedMethod;
    FJavaClassMethod CreateRewardedMethod;
    FJavaClassMethod LoadRewardedMethod;
    FJavaClassMethod DestroyRewardedMethod;
};

#endif
