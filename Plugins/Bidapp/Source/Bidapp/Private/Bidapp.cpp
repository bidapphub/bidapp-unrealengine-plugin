#include "Bidapp.h"
#include "BidappDelegate.h"
#include "BidappLogger.h"
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
    NSString* bannerId;
    NSString* mrecId;
    BOOL logging;
}

-(void)createBannerWithPositionH:(NSString*)horizontalPosition V:(NSString*)verticalPosition;
-(void)showBanner;
-(void)hideBanner;
-(void)destroyBanner;
-(void)startAutorefreshBanner:(int32_t)interval;
-(void)stopAutorefreshBanner;

-(void)createMrecWithPositionH:(NSString*)horizontalPosition V:(NSString*)verticalPosition;
-(void)showMrec;
-(void)hideMrec;
-(void)destroyMrec;
-(void)startAutorefreshMrec:(int32_t)interval;
-(void)stopAutorefreshMrec;

-(void)createInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)loadInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;
-(BOOL)isInterstitialReadyWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)showInterstitialWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)createRewardedWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)loadRewardedWithAdUnitIdentifier:(NSString*)adUnitId;
-(BOOL)isRewardedAdReadyWithAdUnitIdentifier:(NSString*)adUnitId;
-(void)showRewardedAdWithAdUnitIdentifier:(NSString*)adUnitId;

-(void)initialize:(NSString*)pluginVersion sdkKey:(NSString*)sdkKey;
-(BOOL)isInitialized;

-(void)setHasUserConsent:(BOOL)consentGiven;
-(void)setIsAgeRestrictedUser:(BOOL)ageRestricted;
-(void)setDoNotSell:(BOOL)doNotSell;
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
    
    params[@"idt"] = idt;
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

-(void)createBannerWithPositionH:(NSString*)horizontalPosition V:(NSString*)verticalPosition
{
    if (bannerId)
    {
        return;
    }
    
    NSString* position = [self adjustPositionH:horizontalPosition V:verticalPosition];
    Bidapp_createBannerAtPosition_platform("320", position.UTF8String, "320x50");
    bannerId = @"320";
}

-(void)showBanner
{
    if (bannerId)
    {
        Bidapp_showBanner_platform(bannerId.UTF8String);
    }
}

-(void)hideBanner
{
    if (bannerId)
    {
        Bidapp_hideBanner_platform(bannerId.UTF8String);
    }
}

-(void)destroyBanner
{
    if (bannerId)
    {
        Bidapp_destroyBanner_platform(bannerId.UTF8String);
        bannerId = nil;
    }
}

-(void)startAutorefreshBanner:(int32_t)interval
{
    if (bannerId)
    {
        Bidapp_setBannerRefreshInterval_platform(bannerId.UTF8String, ((double)interval)/1000.0);
    }
}

-(void)stopAutorefreshBanner
{
    if (bannerId)
    {
        Bidapp_setBannerRefreshInterval_platform(bannerId.UTF8String, 0.0);
    }
}

-(void)createMrecWithPositionH:(NSString*)horizontalPosition V:(NSString*)verticalPosition
{
    if (mrecId)
    {
        return;
    }
    
    NSString* position = [self adjustPositionH:horizontalPosition V:verticalPosition];
    Bidapp_createBannerAtPosition_platform("300", position.UTF8String, "300x250");
    mrecId = @"300";
}

-(void)showMrec
{
    if (mrecId)
    {
        Bidapp_showBanner_platform(mrecId.UTF8String);
    }
}

-(void)hideMrec
{
    if (mrecId)
    {
        Bidapp_hideBanner_platform(mrecId.UTF8String);
    }
}

-(void)destroyMrec
{
    if (mrecId)
    {
        Bidapp_destroyBanner_platform(mrecId.UTF8String);
        mrecId = nil;
    }
}

-(void)startAutorefreshMrec:(int32_t)interval
{
    if (mrecId)
    {
        Bidapp_setBannerRefreshInterval_platform(mrecId.UTF8String, ((double)interval)/1000.0);
    }
}

-(void)stopAutorefreshMrec
{
    if (mrecId)
    {
        Bidapp_setBannerRefreshInterval_platform(mrecId.UTF8String, 0.0);
    }
}

-(void)createInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_createInterstitial_platform(adUnitId.UTF8String,false);
}

-(void)loadInterstitialWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_loadInterstitial_platform(adUnitId.UTF8String);
}

-(void)createRewardedWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_createRewarded_platform(adUnitId.UTF8String,false);
}

-(void)loadRewardedWithAdUnitIdentifier:(NSString*)adUnitId
{
    Bidapp_loadRewarded_platform(adUnitId.UTF8String);
}

-(void)initialize:(NSString*)pluginVersion appIdValue:(NSString*)appIdValue
{
    NSString* formats = [@[BIDAPP_INTERSTITIAL, BIDAPP_REWARDED, BIDAPP_BANNER]componentsJoinedByString:@" "];
    Bidapp_start_platform2(appIdValue.UTF8String, formats.UTF8String, pluginVersion.UTF8String);
}

-(BOOL)isInitialized
{
    @synchronized(self.class)
    {
        return initialized;
    }
}

-(void)setHasUserConsent:(BOOL)consentGiven
{
    Bidapp_setGDPRConsent_platform(consentGiven);
}

-(void)setIsAgeRestrictedUser:(BOOL)ageRestricted
{
    Bidapp_setSubjectToCOPPA_platform(ageRestricted);
}

-(void)setDoNotSell:(BOOL)doNotSell
{
    Bidapp_setCCPAConsent_platform(!doNotSell);
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


//THIRD_PARTY_INCLUDES_START
//#include <bidapp/bidapp.h>
//THIRD_PARTY_INCLUDES_END

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

bool bEnableBannerAdFormatValue = false;
GConfig->GetBool(
    TEXT("/Script/BidappSettings.BidappSettings"),
    TEXT("bEnableBannerAdFormat"),
    bEnableBannerAdFormatValue,
    GEngineIni
);

bool bEnableInterstitialAdFormatValue = false;
GConfig->GetBool(
    TEXT("/Script/BidappSettings.BidappSettings"),
    TEXT("bEnableInterstitialAdFormat"),
    bEnableInterstitialAdFormatValue,
    GEngineIni
);

bool bEnableRewardAdFormatValue = false;
GConfig->GetBool(
    TEXT("/Script/BidappSettings.BidappSettings"),
    TEXT("bEnableRewardAdFormat"),
    bEnableRewardAdFormatValue,
    GEngineIni
);

if (bSettingSuccess)
{
    UE_LOG(LogTemp, Error, TEXT("Bidapp settings read success"));
    if(bEnableTestModeValue){
        SetTestEnable();
    }
    if (bEnableLoggingValue){
        SetVerboseLoggingEnabled();
    }
    if (bEnableBannerAdFormatValue){
        SetBannerEnable(); 
    }
    if (bEnableInterstitialAdFormatValue){
        SetInterstitialEnable();
    }
    if (bEnableRewardAdFormatValue){
        SetRewardedEnable();
    }
}
else
{
    UE_LOG(LogTemp, Error, TEXT("Bidapp settings read failure"));
    SetVerboseLoggingEnabled();   
    SetRewardedEnable();
    SetInterstitialEnable();
    SetBannerEnable();       
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

void UBidapp::SetHasUserConsent(bool bHasUserConsent)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setHasUserConsent:bHasUserConsent];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetHasUserConsent(bHasUserConsent);
#endif
}


void UBidapp::SetIsAgeRestrictedUser(bool bIsAgeRestrictedUser)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setIsAgeRestrictedUser:bIsAgeRestrictedUser];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetIsAgeRestrictedUser(bIsAgeRestrictedUser);
#endif
}

void UBidapp::SetDoNotSell(bool bDoNotSell)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setDoNotSell:bDoNotSell];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetDoNotSell(bDoNotSell);
#endif
}

void UBidapp::SetInterstitialEnable()
{
#if PLATFORM_IOS
    //[GetIOSPlugin() setInterstitialEnable];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetInterstitialEnable();
#endif
}

void UBidapp::SetRewardedEnable()
{
#if PLATFORM_IOS
    //[GetIOSPlugin() setRewardedEnable];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetRewardedEnable();
#endif
}

void UBidapp::SetBannerEnable()
{
#if PLATFORM_IOS
    //[GetIOSPlugin() SetBannerEnable];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetBannerEnable();
#endif
}

void UBidapp::SetVerboseLoggingEnabled(bool bEnabled)
{
#if PLATFORM_IOS
    [GetIOSPlugin() setVerboseLoggingEnabled:bEnabled];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->SetVerboseLoggingEnabled(bEnabled);
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

void UBidapp::CreateBanner(BBannerVerticalPosition BannerVerticalPosition, BBannerHorizontalPosition BannerHorizontalPosition)
{
const FString BannerVerticalString = GetBannerVerticalString(BannerVerticalPosition);
const FString BannerHorizontalString = GetBannerHorizontalString(BannerHorizontalPosition);
#if PLATFORM_IOS
    [GetIOSPlugin() createBannerWithPositionH:BannerHorizontalString.GetNSString() V:BannerVerticalString.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateBanner(BannerVerticalString, BannerHorizontalString);
#endif
}

void UBidapp::ShowBanner()
{
#if PLATFORM_IOS
    [GetIOSPlugin() showBanner];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowBanner();
#endif
}

void UBidapp::HideBanner()
{
#if PLATFORM_IOS
    [GetIOSPlugin() hideBanner];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->HideBanner();
#endif
}

void UBidapp::DestroyBanner()
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyBanner];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyBanner();
#endif
}

void UBidapp::StartAutorefreshBanner(int32 interval)
{
#if PLATFORM_IOS
    [GetIOSPlugin() startAutorefreshBanner:(int32_t)interval];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StartAutorefreshBanner(interval);
#endif
}

void UBidapp::StopAutorefreshBanner()
{
#if PLATFORM_IOS
    [GetIOSPlugin() stopAutorefreshBanner];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StopAutorefreshBanner();
#endif
}

// Mrec

void UBidapp::CreateMrec(BBannerVerticalPosition MrecVerticalPosition, BBannerHorizontalPosition MrecHorizontalPosition)
{
const FString MRecVerticalString = GetBannerVerticalString(MrecVerticalPosition);
const FString MRecHorizontalString = GetBannerHorizontalString(MrecHorizontalPosition);
#if PLATFORM_IOS
    [GetIOSPlugin() createMrecWithPositionH:MRecHorizontalString.GetNSString() V:MRecVerticalString.GetNSString()];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateMrec(MRecVerticalString, MRecHorizontalString);
#endif
}

void UBidapp::ShowMrec()
{
#if PLATFORM_IOS
    [GetIOSPlugin() showMrec];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowMrec();
#endif
}

void UBidapp::HideMrec()
{
#if PLATFORM_IOS
    [GetIOSPlugin() hideMrec];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->HideBanner();
#endif
}

void UBidapp::DestroyMrec()
{
#if PLATFORM_IOS
    [GetIOSPlugin() destroyMrec];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->DestroyMrec();
#endif
}

void UBidapp::StartAutorefreshMrec(int32 interval)
{
#if PLATFORM_IOS
    [GetIOSPlugin() startAutorefreshMrec:(int32_t)interval];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StartAutorefreshMrec(interval);
#endif
}

void UBidapp::StopAutorefreshMrec()
{
#if PLATFORM_IOS
    [GetIOSPlugin() stopAutorefreshMrec];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->StopAutorefreshMrec();
#endif
}


// Interstitials

void UBidapp::CreateInterstitial(bool autoCaching)
{
#if PLATFORM_IOS
    [GetIOSPlugin() createInterstitialWithAdUnitIdentifier:@"111"];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->CreateInterstitial(autoCaching);
#endif
}

void UBidapp::LoadInterstitial()
{
#if PLATFORM_IOS
    [GetIOSPlugin() loadInterstitialWithAdUnitIdentifier:@"111"];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->LoadInterstitial();
#endif
}

bool UBidapp::IsInterstitialReady()
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isInterstitialReadyWithAdUnitIdentifier:@"111"];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsInterstitialReady();
#else
    return false;
#endif
}


void UBidapp::ShowInterstitial()
{
#if PLATFORM_IOS
    [GetIOSPlugin() showInterstitialWithAdUnitIdentifier:@"111"];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowInterstitial();
#endif
}

void UBidapp::CreateRewarded(bool autoCaching)
{
#if PLATFORM_IOS
    [GetIOSPlugin() createRewardedWithAdUnitIdentifier:@"888"];
#elif PLATFORM_ANDROID
     GetAndroidPlugin()->CreateRewarded(autoCaching);
#endif
}

void UBidapp::LoadRewarded()
{
#if PLATFORM_IOS
    [GetIOSPlugin() loadRewardedWithAdUnitIdentifier:@"888"];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->LoadRewarded();
#endif
}

bool UBidapp::IsRewardedReady()
{
#if PLATFORM_IOS
    return [GetIOSPlugin() isRewardedAdReadyWithAdUnitIdentifier:@"888"];
#elif PLATFORM_ANDROID
    return GetAndroidPlugin()->IsRewardedReady();
#else
    return false;
#endif
}



void UBidapp::ShowRewarded()
{
#if PLATFORM_IOS
    [GetIOSPlugin() showRewardedAdWithAdUnitIdentifier:@"888"];
#elif PLATFORM_ANDROID
    GetAndroidPlugin()->ShowRewarded();
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
            UE_LOG(LogTemp, Warning, TEXT("@@@@@@@@@RewardedDidLoadEvent"));
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
            MAX_USER_WARN("Unknown MAX ad event fired: %s", *Name);
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
