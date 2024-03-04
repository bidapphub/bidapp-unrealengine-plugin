
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
    void SetHasUserConsent(bool bHasUserConsent);
    void SetIsAgeRestrictedUser(bool bIsAgeRestrictedUser);
    void SetDoNotSell(bool bDoNotSell);

    // MARK: General
    void SetVerboseLoggingEnabled();
    bool IsVerboseLoggingEnabled();
    void SetTestEnable();
    void SetRewardedEnable();
    void SetInterstitialEnable();
    void SetBannerEnable();



    // MARK: Banners
    void CreateBanner(const FString &BannerVericalPosition, const FString &BannerHorizontalPosition);
    void ShowBanner();
    void HideBanner();
    void DestroyBanner();
    void StartAutorefreshBanner(int32 interval);
    void StopAutorefreshBanner();

    
    // MARK: Mrec
    void CreateMrec(const FString &MrecVericalPosition, const FString &MrecHorizontalPosition);
    void ShowMrec();
    void HideMrec();
    void DestroyMrec();
    void StartAutorefreshMrec(int32 interval);
    void StopAutorefreshMrec();


    // MARK: Interstitials
    void CreateInterstitial(bool autoCaching);
    bool IsInterstitialReady();
    void ShowInterstitial();
    void LoadInterstitial();

    // MARK: Rewarded
    void CreateRewarded(bool autoCaching);
    bool IsRewardedReady();
    void ShowRewarded();
    void LoadRewarded();

private:
    static FName GetClassName();

    FJavaClassMethod InitializeMethod;
    FJavaClassMethod IsInitializedMethod;

    FJavaClassMethod SetHasUserConsentMethod;
    FJavaClassMethod SetIsAgeRestrictedUserMethod;
    FJavaClassMethod SetDoNotSellMethod;

    FJavaClassMethod SetVerboseLoggingEnabledMethod;
    FJavaClassMethod IsVerboseLoggingEnabledMethod;
    FJavaClassMethod SetTestEnableMethod;
    FJavaClassMethod SetRewardedEnableMethod;
    FJavaClassMethod SetInterstitialEnableMethod;
    FJavaClassMethod SetBannerEnableMethod;
 
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

    FJavaClassMethod IsInterstitialReadyMethod;
    FJavaClassMethod ShowInterstitialMethod;
    FJavaClassMethod CreateInterstitialMethod;
    FJavaClassMethod LoadInterstitialMethod;

    FJavaClassMethod IsRewardedReadyMethod;
    FJavaClassMethod ShowRewardedMethod;
    FJavaClassMethod CreateRewardedMethod;
    FJavaClassMethod LoadRewardedMethod;
};

#endif
