/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#pragma once

#include "AdError.h"
#include "AdInfo.h"
#include "SdkConfiguration.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Bidapp.generated.h"

// Forward declarations for plugin classes
#if PLATFORM_IOS
@class BIDUnrealPlugin;
#elif PLATFORM_ANDROID
class FJavaAndroidBidappUnrealPlugin;
#endif


UENUM()
enum class BBannerVerticalPosition : uint8
{
    Top,
    Center,
    Bottom
};


UENUM()
enum class BBannerHorizontalPosition : uint8
{
    Left,
    Center,
    Right    
};


UCLASS()
class BIDAPP_API UBidapp : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public: 

    static void SetVerboseLoggingEnabled();

    static void SetTestEnable();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void Initialize();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsInitialized();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetGDPR(bool bGDPR);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetCOPPA(bool bCOPPA);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetCCPA(bool bCCPA);   

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsVerboseLoggingEnabled();





    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateBanner(const FString &AdUnitIdentifier, BBannerVerticalPosition vertical = BBannerVerticalPosition::Bottom, BBannerHorizontalPosition horizontal = BBannerHorizontalPosition::Center);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowBanner(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void HideBanner(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyBanner(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StartAutorefreshBanner(const FString &AdUnitIdentifier, int32 interval = 30);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StopAutorefreshBanner(const FString &AdUnitIdentifier);



    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateMrec(const FString &AdUnitIdentifier, BBannerVerticalPosition vertical = BBannerVerticalPosition::Bottom, BBannerHorizontalPosition horizontal = BBannerHorizontalPosition::Center);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowMrec(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void HideMrec(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyMrec(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StartAutorefreshMrec(const FString &AdUnitIdentifier, int32 interval = 30);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StopAutorefreshMrec(const FString &AdUnitIdentifier);

    //Leaderboard

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateLeaderboard(const FString &AdUnitIdentifier, BBannerVerticalPosition vertical = BBannerVerticalPosition::Bottom, BBannerHorizontalPosition horizontal = BBannerHorizontalPosition::Center);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowLeaderboard(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void HideLeaderboard(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyLeaderboard(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StartAutorefreshLeaderboard(const FString &AdUnitIdentifier, int32 interval = 30);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StopAutorefreshLeaderboard(const FString &AdUnitIdentifier);


    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateInterstitial(const FString &AdUnitIdentifier, bool AutoCaching);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsInterstitialReady(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowInterstitial(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void LoadInterstitial(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyInterstitial(const FString &AdUnitIdentifier);

    
    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateRewarded(const FString &AdUnitIdentifier, bool AutoCaching);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsRewardedReady(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowRewarded(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void LoadRewarded(const FString &AdUnitIdentifier);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyRewarded(const FString &AdUnitIdentifier);




    DECLARE_MULTICAST_DELEGATE_OneParam(FSdkInitializedDelegate, const FSdkConfiguration & /*SdkConfiguration*/);


    DECLARE_MULTICAST_DELEGATE_OneParam(FBannerAdViewDidLoadAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FBannerAdViewClickedDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FBannerAdViewDidDisplayAdDelegate, const FAdInfo & /*AdInfo*/); 
    DECLARE_MULTICAST_DELEGATE_TwoParams(FBannerAdViewDidFailToDisplayAdDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FBannerAllNetworksDidFailToDisplayAdDelegate, const FAdError & /*AdError*/); 

    DECLARE_MULTICAST_DELEGATE_OneParam(FMrecAdViewDidLoadAdDelegate, const FAdInfo & /*AdInfo*/);    
    DECLARE_MULTICAST_DELEGATE_OneParam(FMrecAdViewClickedDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FMrecAdViewDidDisplayAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FMrecAdViewDidFailToDisplayAdDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FMrecAllNetworksDidFailToDisplayAdDelegate, const FAdError & /*AdError*/);

    //Leaderboard

    DECLARE_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewDidLoadAdDelegate, const FAdInfo & /*AdInfo*/);    
    DECLARE_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewClickedDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FLeaderboardAdViewDidDisplayAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FLeaderboardAdViewDidFailToDisplayAdDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FLeaderboardAllNetworksDidFailToDisplayAdDelegate, const FAdError & /*AdError*/);
   
    DECLARE_MULTICAST_DELEGATE_OneParam(FInterstitialDidLoadDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FInterstitialDidFailToLoadDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FInterstitialDidDisplayAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FInterstitialDidFailToDisplayAdDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FInterstitialDidHideAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FInterstitialDidClickAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FInterstitialAllNetworksDidFailToDisplayAdDelegate, const FAdError & /*AdError*/);
 
    DECLARE_MULTICAST_DELEGATE_OneParam(FRewardedDidLoadDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FRewardedDidFailToLoadDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FRewardedDidDisplayAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FRewardedDidFailToDisplayAdDelegate, const FAdInfo & /*AdInfo*/, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FRewardedDidHideAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FRewardedDidClickAdDelegate, const FAdInfo & /*AdInfo*/);
    DECLARE_MULTICAST_DELEGATE_OneParam(FRewardedAllNetworksDidFailToDisplayAdDelegate, const FAdError & /*AdError*/);
    DECLARE_MULTICAST_DELEGATE(FRewardedDidRewardUserDelegate);

    static FSdkInitializedDelegate SdkInitializedDelegate;

    static FBannerAdViewDidLoadAdDelegate BannerAdViewDidLoadAdDelegate;
    static FBannerAdViewClickedDelegate BannerAdViewClickedDelegate;
    static FBannerAdViewDidDisplayAdDelegate BannerAdViewDidDisplayAdDelegate;
    static FBannerAdViewDidFailToDisplayAdDelegate BannerAdViewDidFailToDisplayAdDelegate;
    static FBannerAllNetworksDidFailToDisplayAdDelegate BannerAllNetworksDidFailToDisplayAdDelegate;

    static FMrecAdViewDidLoadAdDelegate MrecAdViewDidLoadAdDelegate;
    static FMrecAdViewClickedDelegate MrecAdViewClickedDelegate;
    static FMrecAdViewDidDisplayAdDelegate MrecAdViewDidDisplayAdDelegate;
    static FMrecAdViewDidFailToDisplayAdDelegate MrecAdViewDidFailToDisplayAdDelegate;
    static FMrecAllNetworksDidFailToDisplayAdDelegate MrecAllNetworksDidFailToDisplayAdDelegate;

    //Leaderboard
    static FLeaderboardAdViewDidLoadAdDelegate LeaderboardAdViewDidLoadAdDelegate;
    static FLeaderboardAdViewClickedDelegate LeaderboardAdViewClickedDelegate;
    static FLeaderboardAdViewDidDisplayAdDelegate LeaderboardAdViewDidDisplayAdDelegate;
    static FLeaderboardAdViewDidFailToDisplayAdDelegate LeaderboardAdViewDidFailToDisplayAdDelegate;
    static FLeaderboardAllNetworksDidFailToDisplayAdDelegate LeaderboardAllNetworksDidFailToDisplayAdDelegate;

    static FInterstitialDidLoadDelegate InterstitialDidLoadDelegate;
    static FInterstitialDidFailToLoadDelegate InterstitialDidFailToLoadDelegate;
    static FInterstitialDidDisplayAdDelegate InterstitialDidDisplayAdDelegate;
    static FInterstitialDidFailToDisplayAdDelegate InterstitialDidFailToDisplayAdDelegate;
    static FInterstitialDidHideAdDelegate InterstitialDidHideAdDelegate;
    static FInterstitialDidClickAdDelegate InterstitialDidClickAdDelegate;
    static FInterstitialAllNetworksDidFailToDisplayAdDelegate InterstitialAllNetworksDidFailToDisplayAdDelegate;

    static FRewardedDidLoadDelegate RewardedDidLoadDelegate;
    static FRewardedDidFailToLoadDelegate RewardedDidFailToLoadDelegate;
    static FRewardedDidDisplayAdDelegate RewardedDidDisplayAdDelegate;
    static FRewardedDidFailToDisplayAdDelegate RewardedDidFailToDisplayAdDelegate;
    static FRewardedDidHideAdDelegate RewardedDidHideAdDelegate;
    static FRewardedDidClickAdDelegate RewardedDidClickAdDelegate;
    static FRewardedAllNetworksDidFailToDisplayAdDelegate RewardedAllNetworksDidFailToDisplayAdDelegate;
    static FRewardedDidRewardUserDelegate RewardedDidRewardUserDelegate;

    protected:
    static FString GetBannerVerticalString(BBannerVerticalPosition VerticalPosition);
    static FString GetBannerHorizontalString(BBannerHorizontalPosition HorizontalPosition);

    #if PLATFORM_IOS
    static NSString *GetNSString(const FString &String);
    static NSArray<NSString *> *GetNSArray(const TArray<FString> &Array);
    static NSDictionary<NSString *, NSString *> *GetNSDictionary(const TMap<FString, FString> &Map);
    static BIDUnrealPlugin *GetIOSPlugin();
#elif PLATFORM_ANDROID
    static TSharedPtr<FJavaAndroidBidappUnrealPlugin> GetAndroidPlugin();
#endif

};
