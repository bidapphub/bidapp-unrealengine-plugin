/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "AndroidJavaBidappUnrealPlugin.h"

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

FJavaAndroidBidappUnrealPlugin::FJavaAndroidBidappUnrealPlugin()
    : FJavaClassObject(GetClassName(), "(Landroid/app/Activity;)V", FAndroidApplication::GetGameActivityThis()),
      InitializeMethod(GetClassMethod("initialize", "(Ljava/lang/String;Ljava/lang/String;Lio/bidapp/unreal/BidappUnrealPlugin$EventListener;)V")),
      IsInitializedMethod(GetClassMethod("isInitialized", "()Z")),
      SetGDPRMethod(GetClassMethod("setGDPR", "(Z)V")),
      SetCOPPAMethod(GetClassMethod("setCOPPA", "(Z)V")),
      SetCCPAMethod(GetClassMethod("setCCPA", "(Z)V")),
      SetVerboseLoggingEnabledMethod(GetClassMethod("setVerboseLoggingEnabled", "()V")),
      IsVerboseLoggingEnabledMethod(GetClassMethod("isVerboseLoggingEnabled", "()Z")),
      SetTestEnableMethod(GetClassMethod("setTestEnable", "()V")),
      
      CreateBannerMethod(GetClassMethod("createBanner", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")),
      ShowBannerMethod(GetClassMethod("showBanner", "(Ljava/lang/String;)V")),
      HideBannerMethod(GetClassMethod("hideBanner", "(Ljava/lang/String;)V")),
      StartAutorefreshBannerMethod(GetClassMethod("startAutorefreshBanner", "(Ljava/lang/String;I)V")),
      StopAutorefreshBannerMethod(GetClassMethod("stopAutorefreshBanner", "(Ljava/lang/String;)V")),
      DestroyBannerMethod(GetClassMethod("destroyBanner", "(Ljava/lang/String;)V")),

      CreateMrecMethod(GetClassMethod("createMrec", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")),
      ShowMrecMethod(GetClassMethod("showMrec", "(Ljava/lang/String;)V")),
      HideMrecMethod(GetClassMethod("hideMrec", "(Ljava/lang/String;)V")),
      StartAutorefreshMrecMethod(GetClassMethod("startAutorefreshMrec", "(Ljava/lang/String;I)V")),
      StopAutorefreshMrecMethod(GetClassMethod("stopAutorefreshMrec", "(Ljava/lang/String;)V")),
      DestroyMrecMethod(GetClassMethod("destroyMrec", "(Ljava/lang/String;)V")),

      CreateLeaderboardMethod(GetClassMethod("createLeaderboard", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")),
      ShowLeaderboardMethod(GetClassMethod("showLeaderboard", "(Ljava/lang/String;)V")),
      HideLeaderboardMethod(GetClassMethod("hideLeaderboard", "(Ljava/lang/String;)V")),
      StartAutorefreshLeaderboardMethod(GetClassMethod("startAutorefreshLeaderboard", "(Ljava/lang/String;I)V")),
      StopAutorefreshLeaderboardMethod(GetClassMethod("stopAutorefreshLeaderboard", "(Ljava/lang/String;)V")),
      DestroyLeaderboardMethod(GetClassMethod("destroyLeaderboard", "(Ljava/lang/String;)V")),

      IsInterstitialReadyMethod(GetClassMethod("isInterstitialReady", "(Ljava/lang/String;)Z")),
      ShowInterstitialMethod(GetClassMethod("showInterstitial", "(Ljava/lang/String;)V")),
      CreateInterstitialMethod(GetClassMethod("createInterstitial", "(Ljava/lang/String;Z)V")),
      LoadInterstitialMethod(GetClassMethod("loadInterstitial", "(Ljava/lang/String;)V")),
      DestroyInterstitialMethod(GetClassMethod("destroyInterstitial", "(Ljava/lang/String;)V")),
      CreateRewardedMethod(GetClassMethod("createRewarded", "(Ljava/lang/String;Z)V")),
      LoadRewardedMethod(GetClassMethod("loadRewarded", "(Ljava/lang/String;)V")),
      IsRewardedReadyMethod(GetClassMethod("isRewardedReady", "(Ljava/lang/String;)Z")),
      ShowRewardedMethod(GetClassMethod("showRewarded", "(Ljava/lang/String;)V")),
      DestroyRewardedMethod(GetClassMethod("destroyRewarded", "(Ljava/lang/String;)V"))
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

void FJavaAndroidBidappUnrealPlugin::SetGDPR(bool bGDPR)
{
    CallMethod<void>(SetGDPRMethod, bGDPR);
}

void FJavaAndroidBidappUnrealPlugin::SetCOPPA(bool bCOPPA)
{
    CallMethod<void>(SetCOPPAMethod, bCOPPA);
}


void FJavaAndroidBidappUnrealPlugin::SetCCPA(bool bCCPA)
{
    CallMethod<void>(SetCCPAMethod, bCCPA);
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


// MARK: - Banners

void FJavaAndroidBidappUnrealPlugin::CreateBanner(const FString &AdUnitIdentifier, const FString &BannerVerticalPosition, const FString &BannerHorizontalPosition)
{
    return CallMethod<void>(CreateBannerMethod, *GetJString(AdUnitIdentifier), *GetJString(BannerVerticalPosition), *GetJString(BannerHorizontalPosition));
}

void FJavaAndroidBidappUnrealPlugin::ShowBanner(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(ShowBannerMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::HideBanner(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(HideBannerMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::DestroyBanner(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(DestroyBannerMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::StartAutorefreshBanner(const FString &AdUnitIdentifier, int32 interval)
{
    return CallMethod<void>(StartAutorefreshBannerMethod, *GetJString(AdUnitIdentifier), interval);
}

void FJavaAndroidBidappUnrealPlugin::StopAutorefreshBanner(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(StopAutorefreshBannerMethod, *GetJString(AdUnitIdentifier));
}


// MARK: - Mrec

void FJavaAndroidBidappUnrealPlugin::CreateMrec(const FString &AdUnitIdentifier, const FString &MrecVerticalPosition, const FString &MrecHorizontalPosition)
{
    return CallMethod<void>(CreateMrecMethod, *GetJString(AdUnitIdentifier), *GetJString(MrecVerticalPosition), *GetJString(MrecHorizontalPosition));
}

void FJavaAndroidBidappUnrealPlugin::ShowMrec(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(ShowMrecMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::HideMrec(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(HideMrecMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::DestroyMrec(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(DestroyMrecMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::StartAutorefreshMrec(const FString &AdUnitIdentifier, int32 interval)
{
    return CallMethod<void>(StartAutorefreshMrecMethod, *GetJString(AdUnitIdentifier), interval);
}

void FJavaAndroidBidappUnrealPlugin::StopAutorefreshMrec(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(StopAutorefreshMrecMethod, *GetJString(AdUnitIdentifier));
}



// MARK: - Leaderboard

void FJavaAndroidBidappUnrealPlugin::CreateLeaderboard(const FString &AdUnitIdentifier, const FString &LeaderboardVerticalPosition, const FString &LeaderboardHorizontalPosition)
{
    return CallMethod<void>(CreateLeaderboardMethod, *GetJString(AdUnitIdentifier), *GetJString(LeaderboardVerticalPosition), *GetJString(LeaderboardHorizontalPosition));
}

void FJavaAndroidBidappUnrealPlugin::ShowLeaderboard(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(ShowLeaderboardMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::HideLeaderboard(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(HideLeaderboardMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::DestroyLeaderboard(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(DestroyLeaderboardMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::StartAutorefreshLeaderboard(const FString &AdUnitIdentifier, int32 interval)
{
    return CallMethod<void>(StartAutorefreshLeaderboardMethod, *GetJString(AdUnitIdentifier), interval);
}

void FJavaAndroidBidappUnrealPlugin::StopAutorefreshLeaderboard(const FString &AdUnitIdentifier)
{
    return CallMethod<void>(StopAutorefreshLeaderboardMethod, *GetJString(AdUnitIdentifier));
}





// MARK: - Interstitials


bool FJavaAndroidBidappUnrealPlugin::IsInterstitialReady(const FString &AdUnitIdentifier)
{
    return CallMethod<bool>(IsInterstitialReadyMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::ShowInterstitial(const FString &AdUnitIdentifier)
{
    CallMethod<void>(ShowInterstitialMethod, *GetJString(AdUnitIdentifier));
}
void FJavaAndroidBidappUnrealPlugin::CreateInterstitial(const FString &AdUnitIdentifier, bool autoCaching)
{
    CallMethod<void>(CreateInterstitialMethod, *GetJString(AdUnitIdentifier), autoCaching);
}

void FJavaAndroidBidappUnrealPlugin::LoadInterstitial(const FString &AdUnitIdentifier)
{
    CallMethod<void>(LoadInterstitialMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::DestroyInterstitial(const FString &AdUnitIdentifier)
{
    CallMethod<void>(DestroyInterstitialMethod, *GetJString(AdUnitIdentifier));
}



// MARK: - Rewarded

bool FJavaAndroidBidappUnrealPlugin::IsRewardedReady(const FString &AdUnitIdentifier)
{
    return CallMethod<bool>(IsRewardedReadyMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::ShowRewarded(const FString &AdUnitIdentifier)
{
    CallMethod<void>(ShowRewardedMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::CreateRewarded(const FString &AdUnitIdentifier, bool autoCaching)
{
    CallMethod<void>(CreateRewardedMethod, *GetJString(AdUnitIdentifier), autoCaching);
}

void FJavaAndroidBidappUnrealPlugin::LoadRewarded(const FString &AdUnitIdentifier)
{
    CallMethod<void>(LoadRewardedMethod, *GetJString(AdUnitIdentifier));
}

void FJavaAndroidBidappUnrealPlugin::DestroyRewarded(const FString &AdUnitIdentifier)
{
    CallMethod<void>(DestroyRewardedMethod, *GetJString(AdUnitIdentifier));
}

// MARK: - Private

FName FJavaAndroidBidappUnrealPlugin::GetClassName()
{
    return FName("io/bidapp/unreal/BidappUnrealPlugin");
}

#endif
