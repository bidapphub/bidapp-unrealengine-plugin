/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "Bidapp.h"
#include "BidappDelegate.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Misc/ConfigCacheIni.h"

#if PLATFORM_IOS
#include "IOS/IOSAppDelegate.h"
#include "iOS/BidappSDKPlugin.h"
#import <Foundation/Foundation.h>

typedef void(*event_forwarder_t)(NSString*,NSString*);

@interface BIDUnrealPlugin : NSObject
{
    BOOL logging;
}

-(void)createBannerWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition;
-(void)showBannerWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)hideBannerWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)destroyBannerWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)startAutorefreshBanner:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId;
-(void)stopAutorefreshBannerWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)createMrecWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition;
-(void)showMrecWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)hideMrecWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)destroyMrecWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)startAutorefreshMrec:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId;
-(void)stopAutorefreshMrecWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)createLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition;
-(void)showLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)hideLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)destroyLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)startAutorefreshLeaderboard:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId;
-(void)stopAutorefreshLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)createInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)loadInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;
-(BOOL)isInterstitialReadyWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)showInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)destroyInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)createRewardedWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)loadRewardedWithAdUnitIdentifier:(NSString*)adUnitId;
-(BOOL)isRewardedAdReadyWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)showRewardedAdWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)destroyRewardedWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)initialize:(NSString*)pluginVersion pubIdValue:(NSString*)pubIdValue;
-(BOOL)isInitialized;

-(void)setVerboseLoggingEnabled:(BOOL)enabled;
-(BOOL)isVerboseLoggingEnabled;
-(void)setTestEnable;

-(id)initWithView:(UIView*)mainView eventCallback:(event_forwarder_t)forwarder;

@end

static UIView* mainView;
static event_forwarder_t forwarder = NULL;
static NSMutableSet* interstitialsLoaded = nil;
static NSMutableSet* rewardedsLoaded = nil;
static BOOL initialized = NO;

@implementation BIDUnrealPlugin

typedef BOOL(^adapter_filter_t)(int networkAdapterId);
//To silence compiler. Method of BIDNetworks
+(void)setFilter:(adapter_filter_t)filter_
{
}

+(UIView*)view
{
    return mainView;
}

+(void)onEventName:(NSString*)eventName
                idt:(NSString*)idt
        placementId:(NSString*)placementId
        networkId:(NSString*)networkId
        waterfallId:(NSString*)waterfallId
            revenue:(NSNumber*)revenue
        errorMessage:(NSString*)errorMessage
{
    if (nil == forwarder ||
        nil == eventName ||
        nil == idt)
    {
        return;
    }
    
    if ([eventName isEqualToString:@"InterstitialDidLoadEvent"])
    {
        @synchronized(interstitialsLoaded)
        {
            [interstitialsLoaded addObject:idt];
        }
    }
    else if ([eventName isEqualToString:@"RewardedDidLoadEvent"])
    {
        @synchronized(rewardedsLoaded)
        {
            [rewardedsLoaded addObject:idt];
        }
    }
    else if ([eventName isEqualToString:@"SdkInitEvent"])
    {
        @synchronized(self)
        {
            initialized = YES;
        }
    }
    
    NSMutableDictionary* params = NSMutableDictionary.dictionary;
    
    params[@"AdUnitIdentifier"] = idt;
    params[@"AdUnitPlacement"] = placementId;
    params[@"NetworkId"] = networkId;
    params[@"WaterfallId"] = waterfallId;
    params[@"Revenue"] = revenue.stringValue;
    params[@"Message"] = errorMessage;

    if (!params.count)
    {
        return forwarder(eventName, @"{}");
    }
    
    NSData* d = [NSJSONSerialization dataWithJSONObject:params options:0 error:nil];
    if (!d)
    {
        return forwarder(eventName, @"{}");
    }
    
    NSString* dString = [[NSString alloc]initWithData:d encoding:NSUTF8StringEncoding];
    if (!dString)
    {
        return forwarder(eventName, @"{}");
    }
    
    return forwarder(eventName, dString);
}

-(NSString*)adjustVerticalPosition:(NSString*)verticalPosition
{
    if ([verticalPosition isEqualToString:@"top"])
    {
        return @"0";
    }
    else if ([verticalPosition isEqualToString:@"center"])
    {
        return @"1";
    }
    
    return @"2";
}

-(NSString*)adjustHorizontalPosition:(NSString*)horizontalPosition
{
    if ([horizontalPosition isEqualToString:@"left"])
    {
        return @"0";
    }
    else if ([horizontalPosition isEqualToString:@"right"])
    {
        return @"2";
    }
    
    return @"1";
}

-(NSString*)adjustPositionH:(NSString*)h V:(NSString*)v
{
    return [@[[self adjustHorizontalPosition:h], [self adjustVerticalPosition:v]] componentsJoinedByString:@";"];
}

-(void)createBannerWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition
{
    NSString* position = [self adjustPositionH:horizontalPosition V:verticalPosition];
    Bidapp_createBannerAtPosition_platform(adUnitId.UTF8String, position.UTF8String, "320x50");
}

-(void)showBannerWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_showBanner_platform(adUnitId.UTF8String);
}

-(void)hideBannerWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_hideBanner_platform(adUnitId.UTF8String);
}

-(void)destroyBannerWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_destroyBanner_platform(adUnitId.UTF8String);
}

-(void)startAutorefreshBanner:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, ((double)interval)/1000.0);
}

-(void)stopAutorefreshBannerWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, 0.0);
}

-(void)createMrecWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition
{
    NSString* position = [self adjustPositionH:horizontalPosition V:verticalPosition];
    Bidapp_createBannerAtPosition_platform(adUnitId.UTF8String, position.UTF8String, "300x250");
}

-(void)showMrecWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_showBanner_platform(adUnitId.UTF8String);
}

-(void)hideMrecWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_hideBanner_platform(adUnitId.UTF8String);
}

-(void)destroyMrecWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_destroyBanner_platform(adUnitId.UTF8String);
}

-(void)startAutorefreshMrec:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, ((double)interval)/1000.0);
}

-(void)stopAutorefreshMrecWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, 0.0);
}

-(void)createLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId H:(NSString*)horizontalPosition V:(NSString*)verticalPosition
{
    NSString* position = [self adjustPositionH:horizontalPosition V:verticalPosition];
    Bidapp_createBannerAtPosition_platform(adUnitId.UTF8String, position.UTF8String, "728x90");
}

-(void)showLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_showBanner_platform(adUnitId.UTF8String);
}

-(void)hideLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_hideBanner_platform(adUnitId.UTF8String);
}

-(void)destroyLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_destroyBanner_platform(adUnitId.UTF8String);
}

-(void)startAutorefreshLeaderboard:(int32_t)interval withAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, ((double)interval)/1000.0);
}

-(void)stopAutorefreshLeaderboardWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_setBannerRefreshInterval_platform(adUnitId.UTF8String, 0.0);
}

-(void)createInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_createInterstitial_platform(adUnitId.UTF8String,false);
}

-(void)loadInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_loadInterstitial_platform(adUnitId.UTF8String);
}

-(void)destroyInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_destroyInterstitial_platform(adUnitId.UTF8String);
}

-(void)createRewardedWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_createRewarded_platform(adUnitId.UTF8String,false);
}

-(void)loadRewardedWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_loadRewarded_platform(adUnitId.UTF8String);
}

-(void)destroyRewardedWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_destroyRewarded_platform(adUnitId.UTF8String);
}

-(void)initialize:(NSString*)pluginVersion pubIdValue:(NSString*)pubIdValue
{
    NSString* formats = [@[BIDAPP_INTERSTITIAL, BIDAPP_REWARDED, BIDAPP_BANNER]componentsJoinedByString:@" "];
    Bidapp_start_platform2(pubIdValue.UTF8String, formats.UTF8String, pluginVersion.UTF8String);
}

-(BOOL)isInitialized
{
    @synchronized(self.class)
    {
        return initialized;
    }
}

-(void)setGDPR:(BOOL)consentGiven
{
    Bidapp_setGDPRConsent_platform(consentGiven);
}

-(void)setCOPPA:(BOOL)ageRestricted
{
    Bidapp_setSubjectToCOPPA_platform(ageRestricted);
}

-(void)setCCPA:(BOOL)consentGiven
{
    Bidapp_setCCPAConsent_platform(consentGiven);
}

-(void)setVerboseLoggingEnabled:(BOOL)enabled
{
    if (enabled)
    {
        Bidapp_setLogging_platform(enabled);
        logging = YES;
    }
}

-(BOOL)isVerboseLoggingEnabled
{
    return logging;
}

-(void)setTestEnable
{
    Bidapp_setTestMode_platform(true);
}

-(BOOL)isInterstitialReadyWithAdUnitIdentifier:(NSString*)adUnitId
{
    if (!adUnitId)
    {
        return NO;
    }
    
    @synchronized(interstitialsLoaded)
    {
        return [interstitialsLoaded containsObject:adUnitId];
    }
}

-(void)showInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    if (!adUnitId)
    {
        return;
    }
    
    Bidapp_showInterstitial_platform(adUnitId.UTF8String);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BOOL has = Bidapp_hasInterstitial(adUnitId.UTF8String);
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            
            if (has)
            {
                [interstitialsLoaded addObject:adUnitId];
            }
            else
            {
                [interstitialsLoaded removeObject:adUnitId];
            }
        });
    });
}

-(BOOL)isRewardedAdReadyWithAdUnitIdentifier:(NSString*)adUnitId
{
    if (!adUnitId)
    {
        return NO;
    }
    
    @synchronized(rewardedsLoaded)
    {
        return [rewardedsLoaded containsObject:adUnitId];
    }
}

-(void)showRewardedAdWithAdUnitIdentifier:(NSString*)adUnitId
{
    if (!adUnitId)
    {
        return;
    }

    Bidapp_showRewarded_platform(adUnitId.UTF8String);
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BOOL has = Bidapp_hasRewarded(adUnitId.UTF8String);
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            
            if (has)
            {
                [rewardedsLoaded addObject:adUnitId];
            }
            else
            {
                [rewardedsLoaded removeObject:adUnitId];
            }
        });
    });
}

-(id)initWithView:(UIView*)mainView_ eventCallback:(event_forwarder_t)forwarder_
{
    if (self = [super init])
    {
        mainView = mainView_;
        forwarder = forwarder_;
        interstitialsLoaded = [NSMutableSet set];
        rewardedsLoaded = [NSMutableSet set];
    }
    
    return self;
}

@end


THIRD_PARTY_INCLUDES_START
#include <bidapp/bidapp.h>
THIRD_PARTY_INCLUDES_END

#elif PLATFORM_ANDROID
#include "Android/AndroidJavaBidappUnrealPlugin.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

// MARK: - Initialization

void UBidapp::Initialize()
{
FString PubIdValue = "00000000-0000-0000-0000-000000000000";
bool bSettingSuccess = GConfig->GetString(TEXT("/Script/BidappSettings.BidappSettings"), TEXT("PubId"), PubIdValue, GEngineIni);

    
bool bEnableTestModeValue = false;
GConfig->GetBool(
    TEXT("/Script/BidappSettings.BidappSettings"),
    TEXT("bEnableTestMode"),
    bEnableTestModeValue,
    GEngineIni
);

bool bEnableLoggingValue = false;
GConfig->GetBool(
    TEXT("/Script/BidappSettings.BidappSettings"),
    TEXT("bEnableLogging"),
    bEnableLoggingValue,
    GEngineIni
);
   
    bool bLiftoff = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bLiftoff"),
        bLiftoff,
        GEngineIni
    );

    bool bApplovin = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bApplovin"),
        bApplovin,
        GEngineIni
    );

    bool bApplovinMax = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bApplovinMax"),
        bApplovinMax,
        GEngineIni
    );

    bool bUnity = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bUnity"),
        bUnity,
        GEngineIni
    );

    bool bAdmob = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bAdmob"),
       bAdmob,
        GEngineIni
    );

    bool bChartboost = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bChartboost"),
       bChartboost,
        GEngineIni
    );

    bool bFacebook = false;
    GConfig->GetBool(
        TEXT("/Script/BidappSettings.BidappSettings"),
        TEXT("bFacebook"),
       bChartboost,
        GEngineIni
    );
    
#if PLATFORM_IOS
    [(id)NSClassFromString(@"BIDNetworks") setFilter:^BOOL(int networkAdapterId) {
                           return NO
                           || (bApplovin && networkAdapterId == BIDNetworkId_Applovin)
                           || (bApplovinMax && networkAdapterId == BIDNetworkId_ApplovinMax)
                           || (bUnity && networkAdapterId == BIDNetworkId_Unity)
                           || (bLiftoff && networkAdapterId == BIDNetworkId_Liftoff)
                           || (bChartboost && networkAdapterId == BIDNetworkId_Chartboost)
                           || (bAdmob && networkAdapterId == BIDNetworkId_Admob)
                           || (bFacebook && networkAdapterId == BIDNetworkId_Facebook)
                           ;
    }];
#endif
                           
if (bSettingSuccess)
{
    UE_LOG(LogTemp, Warning, TEXT("Bidapp settings read success"));
    if(bEnableTestModeValue){
        SetTestEnable();
    }
    if (bEnableLoggingValue){
        SetVerboseLoggingEnabled();
    }
}
else
{
    UE_LOG(LogTemp, Error, TEXT("Bidapp settings read failure"));
    SetVerboseLoggingEnabled();   
}        
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("Bidapp");
    FString PluginVersion = Plugin->GetDescriptor().VersionName;

#if PLATFORM_IOS
    [GetIOSPlugin() initialize:PluginVersion.GetNSString() pubIdValue:PubIdValue.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->Initialize(PluginVersion, PubIdValue);
#endif
}



bool UBidapp::IsInitialized()
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isInitialized];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsInitialized();
#else
    return false;
#endif
}

// Privacy

void UBidapp::SetGDPR(bool bGDPR)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setGDPR:bGDPR];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetGDPR(bGDPR);
#endif
}


void UBidapp::SetCOPPA(bool bCOPPA)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setCOPPA:bCOPPA];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetCOPPA(bCOPPA);
#endif
}

void UBidapp::SetCCPA(bool bCCPA)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setCCPA:bCCPA];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetCCPA(bCCPA);
#endif
}

void UBidapp::SetVerboseLoggingEnabled()
{
    bool bEnabled = true;
    
#if PLATFORM_IOS
    [GetIOSPlugin() setVerboseLoggingEnabled:bEnabled];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetVerboseLoggingEnabled();
#endif
}

bool UBidapp::IsVerboseLoggingEnabled()
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isVerboseLoggingEnabled];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsVerboseLoggingEnabled();
#else
    return false;
#endif
}

void UBidapp::SetTestEnable()
{
#if PLATFORM_IOS
    [GetIOSPlugin() setTestEnable];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetTestEnable();
#else
    false;
#endif
}



// Banners

void UBidapp::CreateBanner(const FString &AdUnitIdentifier, BBannerVerticalPosition BannerVerticalPosition, BBannerHorizontalPosition BannerHorizontalPosition)
{
const FString BannerVerticalString = GetBannerVerticalString(BannerVerticalPosition);
const FString BannerHorizontalString = GetBannerHorizontalString(BannerHorizontalPosition);
#if PLATFORM_IOS
    [GetIOSPlugin() createBannerWithAdUnitIdentifier:AdUnitIdentifier.GetNSString() H:BannerHorizontalString.GetNSString() V:BannerVerticalString.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateBanner(AdUnitIdentifier, BannerVerticalString, BannerHorizontalString);
#endif
}

void UBidapp::ShowBanner(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() showBannerWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowBanner(AdUnitIdentifier);
#endif
}

void UBidapp::HideBanner(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() hideBannerWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->HideBanner(AdUnitIdentifier);
#endif
}

void UBidapp::DestroyBanner(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyBannerWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyBanner(AdUnitIdentifier);
#endif
}

void UBidapp::StartAutorefreshBanner(const FString &AdUnitIdentifier, int32 interval)
{
#if PLATFORM_IOS
    [GetIOSPlugin() startAutorefreshBanner:(int32_t)interval withAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StartAutorefreshBanner(AdUnitIdentifier, interval);
#endif
}

void UBidapp::StopAutorefreshBanner(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() stopAutorefreshBannerWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StopAutorefreshBanner(AdUnitIdentifier);
#endif
}

// Mrec

void UBidapp::CreateMrec(const FString &AdUnitIdentifier, BBannerVerticalPosition MrecVerticalPosition, BBannerHorizontalPosition MrecHorizontalPosition)
{
const FString MRecVerticalString = GetBannerVerticalString(MrecVerticalPosition);
const FString MRecHorizontalString = GetBannerHorizontalString(MrecHorizontalPosition);
#if PLATFORM_IOS
    [GetIOSPlugin() createMrecWithAdUnitIdentifier:AdUnitIdentifier.GetNSString() H:MRecHorizontalString.GetNSString() V:MRecVerticalString.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateMrec(AdUnitIdentifier, MRecVerticalString, MRecHorizontalString);
#endif
}

void UBidapp::ShowMrec(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() showMrecWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowMrec(AdUnitIdentifier);
#endif
}

void UBidapp::HideMrec(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() hideMrecWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->HideMrec(AdUnitIdentifier);
#endif
}

void UBidapp::DestroyMrec(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyMrecWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyMrec(AdUnitIdentifier);
#endif
}

void UBidapp::StartAutorefreshMrec(const FString &AdUnitIdentifier, int32 interval)
{
#if PLATFORM_IOS
    [GetIOSPlugin() startAutorefreshMrec:(int32_t)interval withAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StartAutorefreshMrec(AdUnitIdentifier, interval);
#endif
}

void UBidapp::StopAutorefreshMrec(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() stopAutorefreshMrecWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StopAutorefreshMrec(AdUnitIdentifier);
#endif
}


// Leaderboard

void UBidapp::CreateLeaderboard(const FString &AdUnitIdentifier, BBannerVerticalPosition LeaderboardVerticalPosition, BBannerHorizontalPosition LeaderboardHorizontalPosition)
{
const FString LeaderboardVerticalString = GetBannerVerticalString(LeaderboardVerticalPosition);
const FString LeaderboardHorizontalString = GetBannerHorizontalString(LeaderboardHorizontalPosition);
#if PLATFORM_IOS
    [GetIOSPlugin() createLeaderboardWithAdUnitIdentifier:AdUnitIdentifier.GetNSString() H:LeaderboardHorizontalString.GetNSString() V:LeaderboardVerticalString.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateLeaderboard(AdUnitIdentifier, LeaderboardVerticalString, LeaderboardHorizontalString);
#endif
}

void UBidapp::ShowLeaderboard(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() showLeaderboardWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowLeaderboard(AdUnitIdentifier);
#endif
}

void UBidapp::HideLeaderboard(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() hideLeaderboardWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->HideLeaderboard(AdUnitIdentifier);
#endif
}

void UBidapp::DestroyLeaderboard(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyLeaderboardWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyLeaderboard(AdUnitIdentifier);
#endif
}

void UBidapp::StartAutorefreshLeaderboard(const FString &AdUnitIdentifier, int32 interval)
{
#if PLATFORM_IOS
    [GetIOSPlugin() startAutorefreshLeaderboard:interval withAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StartAutorefreshLeaderboard(AdUnitIdentifier, interval);
#endif
}

void UBidapp::StopAutorefreshLeaderboard(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() stopAutorefreshLeaderboardWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StopAutorefreshLeaderboard(AdUnitIdentifier);
#endif
}


// Interstitials

void UBidapp::CreateInterstitial(const FString &AdUnitIdentifier, bool autoCaching)
{
#if PLATFORM_IOS
    [GetIOSPlugin() createInterstitialWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateInterstitial(AdUnitIdentifier, autoCaching);
#endif
}

void UBidapp::LoadInterstitial(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() loadInterstitialWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->LoadInterstitial(AdUnitIdentifier);
#endif
}

bool UBidapp::IsInterstitialReady(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isInterstitialReadyWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsInterstitialReady(AdUnitIdentifier);
#else
    return false;
#endif
}


void UBidapp::ShowInterstitial(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() showInterstitialWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowInterstitial(AdUnitIdentifier);
#endif
}

void UBidapp::DestroyInterstitial(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyInterstitialWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyInterstitial(AdUnitIdentifier);
#endif
}

void UBidapp::CreateRewarded(const FString &AdUnitIdentifier, bool autoCaching)
{
#if PLATFORM_IOS
    [GetIOSPlugin() createRewardedWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
     GetAndroidPlugin()->CreateRewarded(AdUnitIdentifier, autoCaching);
#endif
}

void UBidapp::LoadRewarded(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() loadRewardedWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->LoadRewarded(AdUnitIdentifier);
#endif
}

bool UBidapp::IsRewardedReady(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isRewardedAdReadyWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsRewardedReady(AdUnitIdentifier);
#else
    return false;
#endif
}



void UBidapp::ShowRewarded(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() showRewardedAdWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowRewarded(AdUnitIdentifier);
#endif
}

void UBidapp::DestroyRewarded(const FString &AdUnitIdentifier)
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyRewardedWithAdUnitIdentifier:AdUnitIdentifier.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyRewarded(AdUnitIdentifier);
#endif
}


// Delegates


UBidapp::FSdkInitializedDelegate UBidapp::SdkInitializedDelegate;
UBidapp::FBannerAdViewDidLoadAdDelegate UBidapp::BannerAdViewDidLoadAdDelegate;
UBidapp::FBannerAdViewDidFailToDisplayAdDelegate UBidapp::BannerAdViewDidFailToDisplayAdDelegate;
UBidapp::FBannerAdViewClickedDelegate UBidapp::BannerAdViewClickedDelegate;
UBidapp::FBannerAdViewDidDisplayAdDelegate UBidapp::BannerAdViewDidDisplayAdDelegate;
UBidapp::FBannerAllNetworksDidFailToDisplayAdDelegate UBidapp::BannerAllNetworksDidFailToDisplayAdDelegate;

UBidapp::FMrecAdViewDidLoadAdDelegate UBidapp::MrecAdViewDidLoadAdDelegate;
UBidapp::FMrecAdViewDidFailToDisplayAdDelegate UBidapp::MrecAdViewDidFailToDisplayAdDelegate;
UBidapp::FMrecAdViewClickedDelegate UBidapp::MrecAdViewClickedDelegate;
UBidapp::FMrecAdViewDidDisplayAdDelegate UBidapp::MrecAdViewDidDisplayAdDelegate;
UBidapp::FMrecAllNetworksDidFailToDisplayAdDelegate UBidapp::MrecAllNetworksDidFailToDisplayAdDelegate;

//Leaderboard

UBidapp::FLeaderboardAdViewDidLoadAdDelegate UBidapp::LeaderboardAdViewDidLoadAdDelegate;
UBidapp::FLeaderboardAdViewDidFailToDisplayAdDelegate UBidapp::LeaderboardAdViewDidFailToDisplayAdDelegate;
UBidapp::FLeaderboardAdViewClickedDelegate UBidapp::LeaderboardAdViewClickedDelegate;
UBidapp::FLeaderboardAdViewDidDisplayAdDelegate UBidapp::LeaderboardAdViewDidDisplayAdDelegate;
UBidapp::FLeaderboardAllNetworksDidFailToDisplayAdDelegate UBidapp::LeaderboardAllNetworksDidFailToDisplayAdDelegate;

UBidapp::FInterstitialDidLoadDelegate UBidapp::InterstitialDidLoadDelegate;
UBidapp::FInterstitialDidFailToLoadDelegate UBidapp::InterstitialDidFailToLoadDelegate;
UBidapp::FInterstitialDidDisplayAdDelegate UBidapp::InterstitialDidDisplayAdDelegate;
UBidapp::FInterstitialDidFailToDisplayAdDelegate UBidapp::InterstitialDidFailToDisplayAdDelegate;
UBidapp::FInterstitialDidHideAdDelegate UBidapp::InterstitialDidHideAdDelegate;
UBidapp::FInterstitialDidClickAdDelegate UBidapp::InterstitialDidClickAdDelegate;
UBidapp::FInterstitialAllNetworksDidFailToDisplayAdDelegate UBidapp::InterstitialAllNetworksDidFailToDisplayAdDelegate;

UBidapp::FRewardedDidLoadDelegate UBidapp::RewardedDidLoadDelegate;
UBidapp::FRewardedDidFailToLoadDelegate UBidapp::RewardedDidFailToLoadDelegate;
UBidapp::FRewardedDidDisplayAdDelegate UBidapp::RewardedDidDisplayAdDelegate;
UBidapp::FRewardedDidFailToDisplayAdDelegate UBidapp::RewardedDidFailToDisplayAdDelegate;
UBidapp::FRewardedDidHideAdDelegate UBidapp::RewardedDidHideAdDelegate;
UBidapp::FRewardedDidClickAdDelegate UBidapp::RewardedDidClickAdDelegate;
UBidapp::FRewardedAllNetworksDidFailToDisplayAdDelegate UBidapp::RewardedAllNetworksDidFailToDisplayAdDelegate;
UBidapp::FRewardedDidRewardUserDelegate UBidapp::RewardedDidRewardUserDelegate;

void ForwardEvent(const FString &Name, const FString &Body)
{
    if (Name == TEXT("SdkInitEvent"))
    {
        FSdkConfiguration SdkConfiguration;
        FJsonObjectConverter::JsonObjectStringToUStruct<FSdkConfiguration>(Body, &SdkConfiguration, 0, 0);
        UBidapp::SdkInitializedDelegate.Broadcast(SdkConfiguration);
        UBidappDelegate::BroadcastSdkInitializedEvent(SdkConfiguration);
    }
    else // Ad Events
    {
        FAdInfo AdInfo;
        FJsonObjectConverter::JsonObjectStringToUStruct<FAdInfo>(Body, &AdInfo, 0, 0);

        FAdError AdError;
        FJsonObjectConverter::JsonObjectStringToUStruct<FAdError>(Body, &AdError, 0, 0);

        if (Name == TEXT("BannerAdViewDidLoadAdEvent"))
        {
            UBidapp::BannerAdViewDidLoadAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("BannerAdViewDidFailToDisplayAdEvent"))
        {
            UBidapp::BannerAdViewDidFailToDisplayAdDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("BannerAdViewClickedEvent"))
        {
            UBidapp::BannerAdViewClickedDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("BannerAdViewDidDisplayAdEvent"))
        {
            UBidapp::BannerAdViewDidDisplayAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("BannerAllNetworksDidFailToDisplayAdEvent"))
        {
            UBidapp::BannerAllNetworksDidFailToDisplayAdDelegate.Broadcast(AdError);
            UBidappDelegate::BroadcastAdErrorAllNetworkEvent(Name, AdError);
        }
        else if (Name == TEXT("MrecAdViewDidLoadAdEvent"))
        {
            UBidapp::MrecAdViewDidLoadAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("MrecAdViewDidFailToDisplayAdEvent"))
        {
            UBidapp::MrecAdViewDidFailToDisplayAdDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("MrecAdViewClickedEvent"))
        {
            UBidapp::MrecAdViewClickedDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("MrecAdViewDidDisplayAdEvent"))
        {
            UBidapp::MrecAdViewDidDisplayAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("MrecAllNetworksDidFailToDisplayAdEvent"))
        {
            UBidapp::MrecAllNetworksDidFailToDisplayAdDelegate.Broadcast(AdError);
            UBidappDelegate::BroadcastAdErrorAllNetworkEvent(Name, AdError);
        }
        //Leaderboard
                else if (Name == TEXT("LeaderboardAdViewDidLoadAdEvent"))
        {
            UBidapp::LeaderboardAdViewDidLoadAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("LeaderboardAdViewDidFailToDisplayAdEvent"))
        {
            UBidapp::LeaderboardAdViewDidFailToDisplayAdDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("LeaderboardAdViewClickedEvent"))
        {
            UBidapp::LeaderboardAdViewClickedDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("LeaderboardAdViewDidDisplayAdEvent"))
        {
            UBidapp::LeaderboardAdViewDidDisplayAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("LeaderboardAllNetworksDidFailToDisplayAdEvent"))
        {
            UBidapp::LeaderboardAllNetworksDidFailToDisplayAdDelegate.Broadcast(AdError);
            UBidappDelegate::BroadcastAdErrorAllNetworkEvent(Name, AdError);
        }
        else if (Name == TEXT("InterstitialDidLoadEvent"))
        {
            UBidapp::InterstitialDidLoadDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("InterstitialDidFailToLoadEvent"))
        {
            UBidapp::InterstitialDidFailToLoadDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("InterstitialDidDisplayAdEvent"))
        {
            UBidapp::InterstitialDidDisplayAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("InterstitialDidFailToDisplayAdEvent"))
        {
            UBidapp::InterstitialDidFailToDisplayAdDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("InterstitialDidHideAdEvent"))
        {
            UBidapp::InterstitialDidHideAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("InterstitialDidClickAdEvent"))
        {
            UBidapp::InterstitialDidClickAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("InterstitialAllNetworksDidFailToDisplayAdEvent"))
        {
            UBidapp::InterstitialAllNetworksDidFailToDisplayAdDelegate.Broadcast(AdError);
            UBidappDelegate::BroadcastAdErrorAllNetworkEvent(Name, AdError);
        }
        else if (Name == TEXT("RewardedDidLoadEvent"))
        {
            UBidapp::RewardedDidLoadDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("RewardedDidFailToLoadEvent"))
        {
            UBidapp::RewardedDidFailToLoadDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("RewardedDidDisplayAdEvent"))
        {
            UBidapp::RewardedDidDisplayAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("RewardedDidFailToDisplayAdEvent"))
        {
            UBidapp::RewardedDidFailToDisplayAdDelegate.Broadcast(AdInfo, AdError);
            UBidappDelegate::BroadcastAdErrorEvent(Name, AdInfo, AdError);
        }
        else if (Name == TEXT("RewardedDidHideAdEvent"))
        {
            UBidapp::RewardedDidHideAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("RewardedDidClickAdEvent"))
        {
            UBidapp::RewardedDidClickAdDelegate.Broadcast(AdInfo);
            UBidappDelegate::BroadcastAdEvent(Name, AdInfo);
        }
        else if (Name == TEXT("RewardedAllNetworksDidFailToDisplayAdEvent"))
        {
            UBidapp::RewardedAllNetworksDidFailToDisplayAdDelegate.Broadcast(AdError);
            UBidappDelegate::BroadcastAdErrorAllNetworkEvent(Name, AdError);
        }
        else if (Name == TEXT("RewardedDidRewardUserEvent"))
        {
            UBidapp::RewardedDidRewardUserDelegate.Broadcast();
            UBidappDelegate::BroadcastRewardedDidRewardUserEvent();
        }
        else
        {
           UE_LOG(LogTemp, Warning, TEXT("Unknown Bidapp ad event fired: %s"), *Name);
        }
    }
}



FString UBidapp::GetBannerVerticalString(BBannerVerticalPosition VerticalPosition)
{
    if (VerticalPosition == BBannerVerticalPosition::Top)
    {
        return TEXT("top");
    }
    else if (VerticalPosition == BBannerVerticalPosition::Center)
    {
        return TEXT("center");
    }
    else 
    {
        return TEXT("bottom");
    }
}


FString UBidapp::GetBannerHorizontalString(BBannerHorizontalPosition HorizontalPosition)
{
    if (HorizontalPosition == BBannerHorizontalPosition::Left)
    {
        return TEXT("left");
    }
    else if (HorizontalPosition == BBannerHorizontalPosition::Center)
    {
        return TEXT("center");
    }
    else 
    {
        return TEXT("right");
    }
}


// - IOS

#if PLATFORM_IOS

NSArray<NSString *> *UBidapp::GetNSArray(const TArray<FString> &Array)
{
    NSMutableArray<NSString *> *NewArray = [NSMutableArray arrayWithCapacity:Array.Num()];
    for (const FString &Element : Array)
    {
        [NewArray addObject:Element.GetNSString()];
    }
    return NewArray;
}

NSDictionary<NSString *, NSString *> *UBidapp::GetNSDictionary(const TMap<FString, FString> &Map)
{
    NSMutableDictionary<NSString *, NSString *> *NewDictionary = [NSMutableDictionary dictionaryWithCapacity:Map.Num()];
    for (const TPair<FString, FString> &Entry : Map)
    {
        NewDictionary[Entry.Key.GetNSString()] = Entry.Value.GetNSString();
    }
    return NewDictionary;
}

extern "C" void ForwardIOSEvent(NSString *Name, NSString *Params)
{
    const char *NameCString = [Name UTF8String];
    const char *ParamsCString = [Params UTF8String];
    ForwardEvent((FString(NameCString)), FString(ParamsCString));
}

BIDUnrealPlugin *UBidapp::GetIOSPlugin()
{
    static BIDUnrealPlugin *PluginInstance = nil;
    static dispatch_once_t OnceToken;
    dispatch_once(&OnceToken, ^{
        UIView *MainView = (UIView *)[IOSAppDelegate GetDelegate].IOSView;
        PluginInstance = [[BIDUnrealPlugin alloc] initWithView:MainView eventCallback:&ForwardIOSEvent];
    });
    return PluginInstance;
}

#endif

// Android

#if PLATFORM_ANDROID

void ForwardAndroidEvent(JNIEnv *env, jobject thiz, jstring name, jstring params)
{
    FString Name = FJavaHelper::FStringFromParam(env, name);
    FString Params = FJavaHelper::FStringFromParam(env, params);
    ForwardEvent(Name, Params);
}

// UE5
extern "C" JNIEXPORT void JNICALL Java_com_epicgames_unreal_GameActivity_00024BidappUnrealPluginListener_forwardEvent(JNIEnv *env, jobject thiz, jstring name, jstring params)
{
    ForwardAndroidEvent(env, thiz, name, params);
}

TSharedPtr<FJavaAndroidBidappUnrealPlugin> UBidapp::GetAndroidPlugin()
{
    static TSharedPtr<FJavaAndroidBidappUnrealPlugin, ESPMode::ThreadSafe> Instance;
    if (!Instance.IsValid())
    {
        Instance = MakeShared<FJavaAndroidBidappUnrealPlugin, ESPMode::ThreadSafe>();
    }
    return Instance;
}

#endif
