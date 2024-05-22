/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#pragma once

#include "CoreMinimal.h"
#include "AdError.h"
#include "AdInfo.h"
#include "Components/ActorComponent.h"
#include "SdkConfiguration.h"
#include "BidappDelegate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSdkInitializedDynamicDelegate, const FSdkConfiguration &, SdkConfiguration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBannerAdViewDidLoadAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBannerAdViewClickedDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBannerAdViewDidDisplayAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBannerAdViewDidFailToDisplayAdDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBannerAllNetworksDidFailToDisplayAdDynamicDelegate, const FAdError &, AdError);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMrecAdViewDidLoadAdDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMrecAdViewClickedDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMrecAdViewDidDisplayAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMrecAdViewDidFailToDisplayAdDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMrecAllNetworksDidFailToDisplayAdDynamicDelegate, const FAdError &, AdError);

//Leaderboard

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewDidLoadAdDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewClickedDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewDidDisplayAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLeaderboardAdViewDidFailToDisplayAdDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderboardAllNetworksDidFailToDisplayAdDynamicDelegate, const FAdError &, AdError);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterstitialDidLoadDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInterstitialDidFailToLoadDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterstitialDidDisplayAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInterstitialDidFailToDisplayAdDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterstitialDidHideAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterstitialDidClickAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterstitialAllNetworksDidFailToDisplayAdDynamicDelegate, const FAdError &, AdError);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRewardedDidLoadDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRewardedDidFailToLoadDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRewardedDidDisplayAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRewardedDidFailToDisplayAdDynamicDelegate, const FAdInfo &, AdInfo, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRewardedDidHideAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRewardedDidClickAdDynamicDelegate, const FAdInfo &, AdInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRewardedAllNetworksDidFailToDisplayAdDynamicDelegate, const FAdError &, AdError);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRewardedDidRewardUserDynamicDelegate);

UCLASS(ClassGroup = (Bidapp), DisplayName = "Bidapp Delegate", meta = (BlueprintSpawnableComponent))
class BIDAPP_API UBidappDelegate : public UActorComponent
{
    GENERATED_BODY()

public:
    
    static void BroadcastSdkInitializedEvent(const FSdkConfiguration &SdkConfiguration);
    static void BroadcastAdEvent(const FString &Name, const FAdInfo &AdInfo);
    static void BroadcastAdErrorEvent(const FString &Name, const FAdInfo &AdInfo, const FAdError &AdError);
    static void BroadcastAdErrorAllNetworkEvent(const FString &Name, const FAdError &AdError);
    static void BroadcastRewardedDidRewardUserEvent();

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FSdkInitializedDynamicDelegate SdkInitializedDynamicDelegate;

    // Banners

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FBannerAdViewDidLoadAdDynamicDelegate BannerAdViewDidLoadAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FBannerAdViewClickedDynamicDelegate BannerAdViewClickedDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FBannerAdViewDidDisplayAdDynamicDelegate BannerAdViewDidDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FBannerAdViewDidFailToDisplayAdDynamicDelegate BannerAdViewDidFailToDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FBannerAllNetworksDidFailToDisplayAdDynamicDelegate BannerAllNetworksDidFailToDisplayAdDynamicDelegate;


    // MARK: - MRECs

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FMrecAdViewDidLoadAdDelegate MrecAdViewDidLoadAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FMrecAdViewClickedDynamicDelegate MrecAdViewClickedDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FMrecAdViewDidDisplayAdDynamicDelegate MrecAdViewDidDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FMrecAdViewDidFailToDisplayAdDynamicDelegate MrecAdViewDidFailToDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FMrecAllNetworksDidFailToDisplayAdDynamicDelegate MrecAllNetworksDidFailToDisplayAdDynamicDelegate;

    // MARK: - Leaderboard

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FLeaderboardAdViewDidLoadAdDelegate LeaderboardAdViewDidLoadAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FLeaderboardAdViewClickedDynamicDelegate LeaderboardAdViewClickedDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FLeaderboardAdViewDidDisplayAdDynamicDelegate LeaderboardAdViewDidDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FLeaderboardAdViewDidFailToDisplayAdDynamicDelegate LeaderboardAdViewDidFailToDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FLeaderboardAllNetworksDidFailToDisplayAdDynamicDelegate LeaderboardAllNetworksDidFailToDisplayAdDynamicDelegate;

    // MARK: - Interstitials

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidLoadDynamicDelegate InterstitialDidLoadDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidFailToLoadDynamicDelegate InterstitialDidFailToLoadDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidDisplayAdDynamicDelegate InterstitialDidDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidFailToDisplayAdDynamicDelegate InterstitialDidFailToDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidHideAdDynamicDelegate InterstitialDidHideAdDynamicDelegate;
    
    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialDidClickAdDynamicDelegate InterstitialDidClickAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FInterstitialAllNetworksDidFailToDisplayAdDynamicDelegate InterstitialAllNetworksDidFailToDisplayAdDynamicDelegate;

    // MARK: - Rewarded

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidLoadDynamicDelegate RewardedDidLoadDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidFailToLoadDynamicDelegate RewardedDidFailToLoadDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidDisplayAdDynamicDelegate RewardedDidDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidFailToDisplayAdDynamicDelegate RewardedDidFailToDisplayAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidHideAdDynamicDelegate RewardedDidHideAdDynamicDelegate;
    
    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidClickAdDynamicDelegate RewardedDidClickAdDynamicDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedAllNetworksDidFailToDisplayAdDynamicDelegate RewardedAllNetworksDidFailToDisplayAdDynamicDelegate;
   
    UPROPERTY(BlueprintAssignable, Category = "Bidapp")
    FRewardedDidRewardUserDynamicDelegate RewardedDidRewardUserDynamicDelegate;
};
