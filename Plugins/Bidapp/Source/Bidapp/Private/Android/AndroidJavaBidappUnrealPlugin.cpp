#include "AndroidJavaBidappUnrealPlugin.h"

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

FJavaAndroidBidappUnrealPlugin::FJavaAndroidBidappUnrealPlugin()
    : FJavaClassObject(GetClassName(), "(Landroid/app/Activity;)V", FAndroidApplication::GetGameActivityThis()),
      InitializeMethod(GetClassMethod("initialize", "(Ljava/lang/String;Ljava/lang/String;Lio/bidapp/unreal/BidappUnrealPlugin$EventListener;)V")),
      IsInitializedMethod(GetClassMethod("isInitialized", "()Z")),
      SetHasUserConsentMethod(GetClassMethod("setHasUserConsent", "(Z)V")),
      SetIsAgeRestrictedUserMethod(GetClassMethod("setIsAgeRestrictedUser", "(Z)V")),
      SetDoNotSellMethod(GetClassMethod("setDoNotSell", "(Z)V")),
      SetVerboseLoggingEnabledMethod(GetClassMethod("setVerboseLoggingEnabled", "()V")),
      IsVerboseLoggingEnabledMethod(GetClassMethod("isVerboseLoggingEnabled", "()Z")),
      SetTestEnableMethod(GetClassMethod("setTestEnable", "()V")),
      SetRewardedEnableMethod(GetClassMethod("setRewardedEnable", "()V")),
      SetInterstitialEnableMethod(GetClassMethod("setInterstitialEnable", "()V")),
      SetBannerEnableMethod(GetClassMethod("setBannerEnable", "()V")),
      
      CreateBannerMethod(GetClassMethod("createBanner", "(Ljava/lang/String;Ljava/lang/String;)V")),
      ShowBannerMethod(GetClassMethod("showBanner", "()V")),
      HideBannerMethod(GetClassMethod("hideBanner", "()V")),
      StartAutorefreshBannerMethod(GetClassMethod("startAutorefreshBanner", "(I)V")),
      StopAutorefreshBannerMethod(GetClassMethod("stopAutorefreshBanner", "()V")),
      DestroyBannerMethod(GetClassMethod("destroyBanner", "()V")),

      CreateMrecMethod(GetClassMethod("createMrec", "(Ljava/lang/String;Ljava/lang/String;)V")),
      ShowMrecMethod(GetClassMethod("showMrec", "()V")),
      HideMrecMethod(GetClassMethod("hideMrec", "()V")),
      StartAutorefreshMrecMethod(GetClassMethod("startAutorefreshMrec", "(I)V")),
      StopAutorefreshMrecMethod(GetClassMethod("stopAutorefreshMrec", "()V")),
      DestroyMrecMethod(GetClassMethod("destroyMrec", "()V")),

      IsInterstitialReadyMethod(GetClassMethod("isInterstitialReady", "()Z")),
      ShowInterstitialMethod(GetClassMethod("showInterstitial", "()V")),
      CreateInterstitialMethod(GetClassMethod("createInterstitial", "(Z)V")),
      LoadInterstitialMethod(GetClassMethod("loadInterstitial", "()V")),
      CreateRewardedMethod(GetClassMethod("createRewarded", "(Z)V")),
      LoadRewardedMethod(GetClassMethod("loadRewarded", "()V")),
      IsRewardedReadyMethod(GetClassMethod("isRewardedReady", "()Z")),
      ShowRewardedMethod(GetClassMethod("showRewarded", "()V"))
{
}

FJavaAndroidBidappUnrealPlugin::~FJavaAndroidBidappUnrealPlugin() {}

// MARK: - Initialization

void FJavaAndroidBidappUnrealPlugin::Initialize(const FString &PluginVersion, const FString &SdkKey)
{
    JNIEnv *JEnv = FAndroidApplication::GetJavaEnv();

    // Create listener for Android plugin event handling
    jclass ListenerClass;
    ListenerClass = FAndroidApplication::FindJavaClass("com/epicgames/unreal/GameActivity$BidappUnrealPluginListener");

    jmethodID Constructor = JEnv->GetMethodID(ListenerClass, "<init>", "()V");
    auto LocalListener = NewScopedJavaObject(JEnv, JEnv->NewObject(ListenerClass, Constructor));

    CallMethod<void>(InitializeMethod, *GetJString(PluginVersion), *GetJString(SdkKey), *LocalListener);
}

bool FJavaAndroidBidappUnrealPlugin::IsInitialized()
{
    return CallMethod<bool>(IsInitializedMethod);
}

// MARK: - Privacy

void FJavaAndroidBidappUnrealPlugin::SetHasUserConsent(bool bHasUserConsent)
{
    CallMethod<void>(SetHasUserConsentMethod, bHasUserConsent);
}

void FJavaAndroidBidappUnrealPlugin::SetIsAgeRestrictedUser(bool bIsAgeRestrictedUser)
{
    CallMethod<void>(SetIsAgeRestrictedUserMethod, bIsAgeRestrictedUser);
}


void FJavaAndroidBidappUnrealPlugin::SetDoNotSell(bool bDoNotSell)
{
    CallMethod<void>(SetDoNotSellMethod, bDoNotSell);
}


// MARK: - Terms and Privacy Policy Flow


void FJavaAndroidBidappUnrealPlugin::SetVerboseLoggingEnabled()
{
    CallMethod<void>(SetVerboseLoggingEnabledMethod);
}

bool FJavaAndroidBidappUnrealPlugin::IsVerboseLoggingEnabled()
{
   return CallMethod<bool>(IsVerboseLoggingEnabledMethod);
}

void FJavaAndroidBidappUnrealPlugin::SetTestEnable()
{
    CallMethod<void>(SetTestEnableMethod);
}

void FJavaAndroidBidappUnrealPlugin::SetRewardedEnable()
{
    CallMethod<void>(SetRewardedEnableMethod);
}

void FJavaAndroidBidappUnrealPlugin::SetInterstitialEnable()
{
    CallMethod<void>(SetInterstitialEnableMethod);
}

void FJavaAndroidBidappUnrealPlugin::SetBannerEnable()
{
    CallMethod<void>(SetBannerEnableMethod);
}


// MARK: - Banners

void FJavaAndroidBidappUnrealPlugin::CreateBanner(const FString &BannerVerticalPosition, const FString &BannerHorizontalPosition)
{
    return CallMethod<void>(CreateBannerMethod, *GetJString(BannerVerticalPosition), *GetJString(BannerHorizontalPosition));
}

void FJavaAndroidBidappUnrealPlugin::ShowBanner()
{
    return CallMethod<void>(ShowBannerMethod);
}

void FJavaAndroidBidappUnrealPlugin::HideBanner()
{
    return CallMethod<void>(HideBannerMethod);
}

void FJavaAndroidBidappUnrealPlugin::DestroyBanner()
{
    return CallMethod<void>(DestroyBannerMethod);
}

void FJavaAndroidBidappUnrealPlugin::StartAutorefreshBanner(int32 interval)
{
    return CallMethod<void>(StartAutorefreshBannerMethod, interval);
}

void FJavaAndroidBidappUnrealPlugin::StopAutorefreshBanner()
{
    return CallMethod<void>(StopAutorefreshBannerMethod);
}


// MARK: - Mrec

void FJavaAndroidBidappUnrealPlugin::CreateMrec(const FString &MrecVerticalPosition, const FString &MrecHorizontalPosition)
{
    return CallMethod<void>(CreateMrecMethod, *GetJString(MrecVerticalPosition), *GetJString(MrecHorizontalPosition));
}

void FJavaAndroidBidappUnrealPlugin::ShowMrec()
{
    return CallMethod<void>(ShowMrecMethod);
}

void FJavaAndroidBidappUnrealPlugin::HideMrec()
{
    return CallMethod<void>(HideMrecMethod);
}

void FJavaAndroidBidappUnrealPlugin::DestroyMrec()
{
    return CallMethod<void>(DestroyMrecMethod);
}

void FJavaAndroidBidappUnrealPlugin::StartAutorefreshMrec(int32 interval)
{
    return CallMethod<void>(StartAutorefreshMrecMethod, interval);
}

void FJavaAndroidBidappUnrealPlugin::StopAutorefreshMrec()
{
    return CallMethod<void>(StopAutorefreshMrecMethod);
}




// MARK: - Interstitials


bool FJavaAndroidBidappUnrealPlugin::IsInterstitialReady()
{
    return CallMethod<bool>(IsInterstitialReadyMethod);
}

void FJavaAndroidBidappUnrealPlugin::ShowInterstitial()
{
    CallMethod<void>(ShowInterstitialMethod);
}
void FJavaAndroidBidappUnrealPlugin::CreateInterstitial(bool autoCaching)
{
    CallMethod<void>(CreateInterstitialMethod, autoCaching);
}

void FJavaAndroidBidappUnrealPlugin::LoadInterstitial()
{
    CallMethod<void>(LoadInterstitialMethod);
}


// MARK: - Rewarded

bool FJavaAndroidBidappUnrealPlugin::IsRewardedReady()
{
    return CallMethod<bool>(IsRewardedReadyMethod);
}

void FJavaAndroidBidappUnrealPlugin::ShowRewarded()
{
    CallMethod<void>(ShowRewardedMethod);
}

void FJavaAndroidBidappUnrealPlugin::CreateRewarded(bool autoCaching)
{
    CallMethod<void>(CreateRewardedMethod, autoCaching);
}

void FJavaAndroidBidappUnrealPlugin::LoadRewarded()
{
    CallMethod<void>(LoadRewardedMethod);
}


// MARK: - Private

FName FJavaAndroidBidappUnrealPlugin::GetClassName()
{
    return FName("io/bidapp/unreal/BidappUnrealPlugin");
}

#endif
