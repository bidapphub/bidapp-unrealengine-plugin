

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

    static void SetInterstitialEnable();

    static void SetRewardedEnable();

    static void SetBannerEnable();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void Initialize();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsInitialized();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetHasUserConsent(bool bHasUserConsent);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetIsAgeRestrictedUser(bool bIsAgeRestrictedUser);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void SetDoNotSell(bool bDoNotSell);   

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsVerboseLoggingEnabled();





    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateBanner(BBannerVerticalPosition vertical = BBannerVerticalPosition::Bottom, BBannerHorizontalPosition horizontal = BBannerHorizontalPosition::Center);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowBanner();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void HideBanner();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyBanner();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StartAutorefreshBanner(int32 interval = 30);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StopAutorefreshBanner();



    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateMrec(BBannerVerticalPosition vertical = BBannerVerticalPosition::Bottom, BBannerHorizontalPosition horizontal = BBannerHorizontalPosition::Center);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowMrec();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void HideMrec();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void DestroyMrec();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StartAutorefreshMrec(int32 interval = 30);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void StopAutorefreshMrec();



    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateInterstitial(bool AutoCaching);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsInterstitialReady();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowInterstitial();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void LoadInterstitial();

    
    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void CreateRewarded(bool AutoCaching);

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static bool IsRewardedReady();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void ShowRewarded();

    UFUNCTION(BlueprintCallable, Category = "Bidapp")
    static void LoadRewarded();




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
