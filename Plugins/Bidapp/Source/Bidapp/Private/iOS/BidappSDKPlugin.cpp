/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "BidappSDKPlugin.h"
#include "CoreMinimal.h"

#if PLATFORM_IOS

#import <UIKit/UIKit.h>
#import <Bidapp/BIDAdInfo.h>

@interface BidappStub

+(id)getInstance;
- (void)sendMessageToGOWithName:(const char*)goName functionName:(const char*)name message:(const char*)msg;
- (void)pause:(bool)pause;
- (id)appController;
- (UIViewController*)rootViewController;
+(void)onEventName:(NSString*)eventName
                idt:(NSString*)idt
        placementId:(NSString*)placementId
        networkId:(NSString*)networkId
        waterfallId:(NSString*)waterfallId
            revenue:(NSNumber*)revenue
    errorMessage:(NSString*)errorMessage;

@end

UIViewController* UnrealGetGLViewController_bidapp()
{
    return [UIApplication sharedApplication].keyWindow.rootViewController;
}

static int messageCounter = 0;
static NSString* currentString = @"";

void UnrealSendMessage_bidapp(NSString* message, NSString* idt, BIDAdInfo *adInfo, NSString* error)
{
    Class c = NSClassFromString(@"BIDUnrealPlugin");
    
    NSString* networkId = adInfo ? @(adInfo.networkId).stringValue : nil;
    
    [c onEventName:message
               idt:idt
       placementId:(adInfo ? @"default" : nil)
       networkId:networkId
       waterfallId:adInfo.waterfallId
           revenue:adInfo.revenue
      errorMessage:error];
}

void UnrealPause_bidapp(int pause)
{
    Class cUnrealFramework = NSClassFromString(@"UnrealFramework");
    
    if (![cUnrealFramework respondsToSelector:@selector(getInstance)])
    {
        return;
    }
    
    id i = [cUnrealFramework getInstance];
    
    if (![i respondsToSelector:@selector(pause:)])
    {
        return;
    }
    
    [i pause:pause];
}

#include <bidapp/BidappAds.h>
#include <Bidapp/BIDInterstitial.h>
#include <Bidapp/BIDRewarded.h>
#include <Bidapp/BIDBannerView.h>
#include <Bidapp/BIDConfiguration.h>
#include <Bidapp/BIDConsent.h>
#include <Bidapp/BIDAdFormat.h>
#import <objc/runtime.h>
#import <dlfcn.h>

#if __has_include(<AppTrackingTransparency/AppTrackingTransparency.h>)
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#endif

#define BIDAPP_onSdkInitEvent (@"SdkInitEvent")

#define BIDAPP_onInterstitialDidLoadAd (@"InterstitialDidLoadEvent")
#define BIDAPP_onInterstitialDidFailToLoadAd (@"InterstitialDidFailToLoadEvent")

#define BIDAPP_onInterstitialDidDisplayAd (@"InterstitialDidDisplayAdEvent")
#define BIDAPP_onInterstitialDidClickAd (@"InterstitialDidClickAdEvent")
#define BIDAPP_onInterstitialDidHideAd (@"InterstitialDidHideAdEvent")
#define BIDAPP_onInterstitialDidFailToDisplayAd (@"InterstitialDidFailToDisplayAdEvent")
#define BIDAPP_onInterstitialAllNetworksFailedToDisplayAd (@"InterstitialAllNetworksDidFailToDisplayAdEvent")

#define BIDAPP_onRewardedDidLoadAd (@"RewardedDidLoadEvent")
#define BIDAPP_onRewardedDidFailToLoadAd (@"RewardedDidFailToLoadEvent")

#define BIDAPP_onRewardedDidDisplayAd (@"RewardedDidDisplayAdEvent")
#define BIDAPP_onRewardedDidClickAd (@"RewardedDidClickAdEvent")
#define BIDAPP_onRewardedDidHideAd (@"RewardedDidHideAdEvent")
#define BIDAPP_onRewardedDidFailToDisplayAd (@"RewardedDidFailToDisplayAdEvent")
#define BIDAPP_onRewardedAllNetworksFailedToDisplayAd (@"RewardedAllNetworksDidFailToDisplayAdEvent")
#define BIDAPP_onUserDidReceiveReward (@"RewardedDidRewardUserEvent")

#define BIDAPP_onBannerDidDisplayAd (@"BannerAdViewDidDisplayAdEvent")
#define BIDAPP_onBannerFailedToDisplayAd (@"BannerAdViewDidFailToDisplayAdEvent")
#define BIDAPP_onBannerClicked (@"BannerAdViewClickedEvent")
#define BIDAPP_onBannerAllNetworksFailedToDisplayAd (@"BannerAllNetworksDidFailToDisplayAdEvent")

static NSString* ToNSString(const char* pStr)
{
    if (pStr)
    {
        return [NSString stringWithUTF8String:pStr];
    }
    return nil;
}

static const char* FromNSString(NSString *nsString)
{
    const char *chString = [nsString UTF8String];
    if (chString == NULL)
        return NULL;
    char *resultString = (char*)malloc(strlen(chString) + 1);
    strcpy(resultString, chString);
    return resultString;
}

@interface BidappSDKPluginName : NSString

+(NSString*)pluginName;
+(NSString*)pluginVersion;

@end

@implementation BidappSDKPluginName

char* unityBidappSDK_targetName = NULL;

+(NSString*)pluginName
{
    return BIDAPPSDK_PLUGIN_NAME;
}

static NSString* pluginVersion_ = nil;
+(NSString*)pluginVersion
{
    return pluginVersion_;
}

static NSMutableDictionary<NSString*,BIDBannerView*>* banners = nil;
static NSMutableDictionary<NSString*,NSString*>* bannerPositions = nil;

NSString* identifierForBanner(BIDBannerView *adView)
{
    for (NSString* s in banners)
    {
        if (banners[s] == adView)
        {
            return s;
        }
    }
    
    return @"";
}

+ (void)bannerDidDisplay:(BIDBannerView *)adView adInfo:(BIDAdInfo *)adInfo
{
    NSString* idt = identifierForBanner(adView);
    
    NSString* message = BIDAPP_onBannerDidDisplayAd;
    if (adView.bounds.size.height == 50.0)
    {
    }
    else if (adView.bounds.size.height == 250.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Mrec"];
    }
    else if (adView.bounds.size.height == 90.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Leaderboard"];
    }
    else
    {
        return;
    }
    
    UnrealSendMessage_bidapp(message, idt, adInfo, nil);
}

+ (void)bannerDidFailToDisplay:(BIDBannerView *)adView adInfo:(BIDAdInfo *)adInfo error:(NSError *)error
{
    NSString* errorDescription_ = error.localizedDescription ? error.localizedDescription : @"Unknown error";
    NSString* message = BIDAPP_onBannerFailedToDisplayAd;
    if (adView.bounds.size.height == 50.0)
    {
    }
    else if (adView.bounds.size.height == 250.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Mrec"];
    }
    else if (adView.bounds.size.height == 90.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Leaderboard"];
    }
    else
    {
        return;
    }
    
    UnrealSendMessage_bidapp(message, identifierForBanner(adView), adInfo, errorDescription_);
}

+ (void)bannerDidClick:(BIDBannerView *)adView adInfo:(BIDAdInfo *)adInfo
{
    NSString* message = BIDAPP_onBannerClicked;
    if (adView.bounds.size.height == 50.0)
    {
    }
    else if (adView.bounds.size.height == 250.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Mrec"];
    }
    else if (adView.bounds.size.height == 90.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Leaderboard"];
    }
    else
    {
        return;
    }
    
    UnrealSendMessage_bidapp(message, identifierForBanner(adView), adInfo, nil);
}

+ (void)allNetworksFailedToDisplayAdInBanner:(BIDBannerView *)adView
{
    NSString* errorDescription_ = @"All ad networks failed to display ad";
    NSString* message = BIDAPP_onBannerAllNetworksFailedToDisplayAd;
    if (adView.bounds.size.height == 50.0)
    {
    }
    else if (adView.bounds.size.height == 250.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Mrec"];
    }
    else if (adView.bounds.size.height == 90.0)
    {
        message = [message stringByReplacingOccurrencesOfString:@"Banner" withString:@"Leaderboard"];
    }
    else
    {
        return;
    }
    
    UnrealSendMessage_bidapp(message, identifierForBanner(adView), nil, errorDescription_);
}

@end

@interface BIDInterstitialWrapper : NSObject<BIDInterstitialDelegate,BIDFullscreenLoadDelegate>
{
    BIDInterstitial* interstitial;
    NSString* idt;
}

@end

@implementation BIDInterstitialWrapper

-(id)initWithIdentifier:(NSString*)idt_ autoload:(BOOL)autoload_
{
    if (self = [super init])
    {
        idt = idt_;
        interstitial = [BIDInterstitial new];
        interstitial.loadDelegate = self;
        interstitial.autoload = autoload_;
    }
    
    return self;
}

- (void)didLoadAd:(BIDAdInfo*)adInfo
{
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidLoadAd, idt, adInfo, nil);
}

- (void)didFailToLoadAd:(BIDAdInfo*)adInfo error:(NSError *)error
{
    NSString* errorDescription_ = error.localizedDescription;
    
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidFailToLoadAd, idt, adInfo, errorDescription_);
}

-(BOOL)isAdReady
{
    return interstitial.isAdReady;
}

-(void)load
{
    [interstitial load];
}

-(void)show
{
    [interstitial showWithDelegate:self];
}

- (nonnull UIViewController*)viewControllerForDisplayAd
{
    return UnrealGetGLViewController_bidapp();
}

- (void)didDisplayAd:(BIDAdInfo*)adInfo
{
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidDisplayAd, idt, adInfo, nil);
}

- (void)didClickAd:(BIDAdInfo*)adInfo
{
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidClickAd, idt, adInfo, nil);
}

- (void)didHideAd:(BIDAdInfo*)adInfo
{
    Bidapp_pause_platform(false);

    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidHideAd, idt, adInfo, nil);
}

- (void)didFailToDisplayAd:(BIDAdInfo*)adInfo error:(NSError *)error
{
    NSString* errorDescription_ = error.localizedDescription;
    
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialDidFailToDisplayAd, idt, adInfo, errorDescription_);
}

-(void)allNetworksDidFailToDisplayAd
{
    NSString* errorDescription_ = @"All ad networks failed to display ad";
    
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialAllNetworksFailedToDisplayAd, idt, nil, errorDescription_);
}

@end

@interface BIDRewardedWrapper : NSObject<BIDRewardedDelegate,BIDFullscreenLoadDelegate>
{
    BIDRewarded* rewarded;
    NSString* idt;
}

@end

@implementation BIDRewardedWrapper

-(id)initWithIdentifier:(NSString*)idt_  autoload:(BOOL)autoload_
{
    if (self = [super init])
    {
        idt = idt_;
        rewarded = [BIDRewarded new];
        rewarded.loadDelegate = self;
        rewarded.autoload = autoload_;
    }
    
    return self;
}

- (void)didLoadAd:(BIDAdInfo*)adInfo
{
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidLoadAd, idt, adInfo, nil);
}

- (void)didFailToLoadAd:(BIDAdInfo*)adInfo error:(NSError *)error
{
    NSString* errorDescription_ = error.localizedDescription;
    
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidFailToLoadAd, idt, adInfo, errorDescription_);
}

-(BOOL)isAdReady
{
    return rewarded.isAdReady;
}

-(void)load
{
    [rewarded load];
}

-(void)show
{
    [rewarded showWithDelegate:self];
}

- (nonnull UIViewController*)viewControllerForDisplayAd
{
    return UnrealGetGLViewController_bidapp();
}

- (void)didDisplayAd:(BIDAdInfo*)adInfo
{
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidDisplayAd, idt, adInfo, nil);
}

- (void)didClickAd:(BIDAdInfo*)adInfo
{
    NSString* networkId = @(adInfo.networkId).stringValue;
    NSString* waterfallId = adInfo.waterfallId ? adInfo.waterfallId : @"";
    NSString* revenue = adInfo.revenue ? adInfo.revenue.stringValue : @"0";
    
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidClickAd, idt, adInfo, nil);
}

- (void)didHideAd:(BIDAdInfo*)adInfo
{
    Bidapp_pause_platform(false);
    
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidHideAd, idt, adInfo, nil);
}

- (void)didFailToDisplayAd:(BIDAdInfo*)adInfo error:(NSError *)error
{
    NSString* errorDescription_ = error.localizedDescription;
    
    UnrealSendMessage_bidapp(BIDAPP_onRewardedDidHideAd, idt, adInfo, errorDescription_);
}

-(void)allNetworksDidFailToDisplayAd
{
    NSString* errorDescription_ = @"All ad networks failed to display ad";
    
    UnrealSendMessage_bidapp(BIDAPP_onInterstitialAllNetworksFailedToDisplayAd, idt, nil, errorDescription_);
}

#pragma mark - BIDRewardedDelegate

- (void)didRewardUser
{
    UnrealSendMessage_bidapp(BIDAPP_onUserDidReceiveReward, idt, nil, nil);
}

@end

void Bidapp_setUnrealCallbackTargetName_platform(const char* targetName);

void Bidapp_setUnrealCallbackTargetName_platform(const char* targetName)
{
    if (NULL != targetName)
    {
        char* _old = unityBidappSDK_targetName;
        
        char* _unityBidappSDK_targetName = (char*)malloc(strlen(targetName) + 1);
        
        strcpy(_unityBidappSDK_targetName, targetName);
        
        unityBidappSDK_targetName = _unityBidappSDK_targetName;
        
        if (NULL != _old)
        {
            free(_old);
        }
    }
    
    NSLog(@"Setting unity callback for object with name: %s", unityBidappSDK_targetName);
}

static BIDConfiguration* config()
{
    static BIDConfiguration* config = nil;
    if (!config)
    {
        config = [BIDConfiguration new];
    }
        
    return config;
}

void Bidapp_start_platform_(NSString* pubid, NSString* formats, NSString* version)
{
    pluginVersion_ = version;

    Bidapp_requestTrackingAuthorization_platform();
    
    //if (@available(iOS 14, *)) {
    //    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 1 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
    //        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus s){}];
            
    //        [BidappAds startWithPubid:ToNSString(pubid) config:config()];
    //    });
    //}
    //else {
        [BidappAds startWithPubid:pubid config:config()];
    //}
}


void Bidapp_start_platform2(const char* pubid, const char* formats, const char* version)
{
    NSString* pubid_ = ToNSString(pubid);
    NSString* formats_ = ToNSString(formats);
    NSString* version_ = ToNSString(version);
    
    if (![NSThread isMainThread])
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            
            Bidapp_start_platform_(pubid_,formats_,version_);
        });
        
        return;
    }
    
    Bidapp_start_platform_(pubid_,formats_,version_);
    UnrealSendMessage_bidapp(BIDAPP_onSdkInitEvent, nil, nil, nil);
}

void Bidapp_start_platform(const char* pubid, const char* formats)
{
    Bidapp_start_platform2(pubid, formats, "1.0.0");
}

void Bidapp_setLogging_platform(bool logging)
{
    if (![NSThread isMainThread])
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            
            Bidapp_setLogging_platform(logging);
        });
        
        return;
    }
    
    if (logging)
    {
        [config() enableLogging];
    }
}

void Bidapp_setTestMode_platform(bool testMode)
{
    if (![NSThread isMainThread])
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            
            Bidapp_setTestMode_platform(testMode);
        });
        
        return;
    }
    
    if (testMode)
    {
        [config() enableTestMode];
    }
}

static int theidt = 1;
static int createIdentifier(void)
{
    @synchronized ([BidappSDKPluginName class])
    {
        return ++theidt;
    }
}

static NSMutableDictionary* interstitials = nil;
void Bidapp_createInterstitial_platform_(NSString* identifier, BOOL autoload)
{
    if (!interstitials)
    {
        interstitials = [NSMutableDictionary new];
    }
    
    [interstitials setObject:[[BIDInterstitialWrapper alloc]initWithIdentifier:identifier autoload:autoload] forKey:identifier];
}

void Bidapp_createInterstitial_platform(const char* idt, bool autoload)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        Bidapp_createInterstitial_platform_(identifier, autoload);
    });
}

void Bidapp_loadInterstitial_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{

        BIDInterstitialWrapper* i = interstitials[identifier];
        [i load];
    });
}

void Bidapp_destroyInterstitial_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [interstitials removeObjectForKey:identifier];
    });
}

void Bidapp_showInterstitial_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{

        BIDInterstitialWrapper* i = interstitials[identifier];
        [i show];
    });
}

static NSMutableDictionary* rewardeds = nil;
void Bidapp_createRewarded_platform_(NSString* identifier,BOOL autoload)
{
    if (!rewardeds)
    {
        rewardeds = [NSMutableDictionary new];
    }
    
    [rewardeds setObject:[[BIDRewardedWrapper alloc]initWithIdentifier:identifier autoload:autoload] forKey:identifier];
}

void Bidapp_createRewarded_platform(const char* idt, bool autoload)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        Bidapp_createRewarded_platform_(identifier, autoload);
    });
}

void Bidapp_loadRewarded_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{

        BIDRewardedWrapper* r = rewardeds[identifier];
        [r load];
    });
}

void Bidapp_destroyRewarded_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [rewardeds removeObjectForKey:identifier];
    });
}

void Bidapp_showRewarded_platform(const char* idt)
{
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{

        BIDRewardedWrapper* i = rewardeds[identifier];
        [i show];
    });
}

void Bidapp_showBannerX_platform_(NSString* idt, NSString* bannerSize, float x, float y, float width, float height)
{
    BIDBannerView *banner_view = nil;
    if ([bannerSize isEqualToString:@"300x250"]) {
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_300x250] delegate:(id)[BidappSDKPluginName class]];
    }
    else if ([bannerSize isEqualToString:@"320x50"]){
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_320x50] delegate:(id)[BidappSDKPluginName class]];
    }
    else if ([bannerSize isEqualToString:@"728x90"]){
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_728x90] delegate:(id)[BidappSDKPluginName class]];
    }
    else
    {
        return;
    }
    
    [banner_view stopAutorefresh];
    
    banner_view.frame = CGRectMake(x, y, width, height);
    
    if (!banners)
    {
        banners = [NSMutableDictionary new];
        bannerPositions = [NSMutableDictionary new];
    }

    banners[idt] = banner_view;
    
    [BIDAPP_PLUGIN_VIEW_SOURCE addSubview:banner_view];
}

const char* Bidapp_showBannerX_platform(const char* bannerSize, float x, float y, float width, float height)
{
    NSString* bannerSize_ = ToNSString(bannerSize);
    
    int idt = createIdentifier();
    NSString* identifier = @(idt).stringValue;

    dispatch_async(dispatch_get_main_queue(), ^{
        
        Bidapp_showBannerX_platform_(identifier,bannerSize_,x,y,width,height);
    });
    
    return FromNSString(identifier);
}

NSArray<NSLayoutConstraint *>* constraintsForBanner(UIView* banner, UIView* superView, UIEdgeInsets _insets, NSString* _position, CGSize _size)
{
    NSMutableArray *constraints = [NSMutableArray new];
    NSArray* c = [_position componentsSeparatedByString:@";"];
    int xPosition = [c.firstObject intValue];
    int yPosition = [c.lastObject intValue];
    
    [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:_size.width]];
    [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:_size.height]];
    
    // position horizontally
    switch (xPosition)
    {
        case 1://Center
            [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:superView attribute:NSLayoutAttributeCenterX multiplier:1.0 constant:0]];
            break;
        case 0://Left
            [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:superView attribute:NSLayoutAttributeLeft multiplier:1.0 constant:_insets.left]];
            break;
        case 2://Right
            [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:superView attribute:NSLayoutAttributeRight multiplier:1.0 constant:-_insets.right]];
            break;
            
        default:
            break;
    }
    
    // position vertically
    switch (yPosition)
    {
        case 1://Center
            [constraints addObject:[NSLayoutConstraint constraintWithItem:banner attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:superView attribute:NSLayoutAttributeCenterY multiplier:1.0 constant:0]];
            break;
        case 2://Bottom
            [constraints addObject:[banner.bottomAnchor constraintEqualToAnchor:superView.safeAreaLayoutGuide.bottomAnchor constant:-_insets.bottom]];
            break;
        case 0://Top
            [constraints addObject:[banner.topAnchor constraintEqualToAnchor:superView.safeAreaLayoutGuide.topAnchor  constant:_insets.top]];
            break;
            
        default:
            break;
    }
    
    return constraints;
}

void Bidapp_createBannerAtPosition_platform_(NSString* idt, NSString* position, NSString* bannerSize)
{
    BIDBannerView *banner_view = nil;
    if ([bannerSize isEqualToString:@"300x250"]) {
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_300x250] delegate:(id)[BidappSDKPluginName class]];
    }
    else if ([bannerSize isEqualToString:@"320x50"]){
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_320x50] delegate:(id)[BidappSDKPluginName class]];
    }
    else if ([bannerSize isEqualToString:@"728x90"]){
        banner_view = [BIDBannerView bannerWithFormat:[BIDAdFormat banner_728x90] delegate:(id)[BidappSDKPluginName class]];
    }
    else
    {
        return;
    }

    [banner_view stopAutorefresh];
    
    if (!banners)
    {
        banners = [NSMutableDictionary new];
        bannerPositions = [NSMutableDictionary new];
    }

    banners[idt] = banner_view;
    bannerPositions[idt] = position ? position : @"1;2";
}

void Bidapp_createBannerAtPosition_platform(const char* idt, const char* position, const char* bannerSize)
{
    NSString* position_ = ToNSString(position);
    NSString* bannerSize_ = ToNSString(bannerSize);
    
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        Bidapp_createBannerAtPosition_platform_(identifier,position_,bannerSize_);
    });
}

void Bidapp_showBanner_platform(const char* bannerId)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BIDBannerView *banner_view = banners[identifier];
        if (!banner_view)
        {
            return;
        }
        
        NSString* position = bannerPositions[identifier];
        if (!position)
        {
            return;
        }
        
        [BIDAPP_PLUGIN_VIEW_SOURCE addSubview:banner_view];

        banner_view.translatesAutoresizingMaskIntoConstraints = NO;
        NSArray* constraints = constraintsForBanner(banner_view, BIDAPP_PLUGIN_VIEW_SOURCE, UIEdgeInsetsMake(0.0, 0.0, 0.0, 0.0), position, banner_view.bounds.size);

        [banner_view.superview addConstraints:constraints];

        for (id item in constraints) {
            NSLayoutConstraint *constraint = (NSLayoutConstraint *)item;
            [constraint setActive:YES];
        }
    });
}

void Bidapp_hideBanner_platform(const char* bannerId)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [banners[identifier] removeFromSuperview];
    });
}

void Bidapp_stopBannerAutorefresh_platform(const char* bannerId)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [banners[identifier] stopAutorefresh];
    });
}

void Bidapp_setBannerRefreshInterval_platform(const char* bannerId, double refreshInterval)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [banners[identifier] startAutorefresh:refreshInterval];
    });
}

void Bidapp_destroyBanner_platform(const char* bannerId)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [banners[identifier] refreshAd];
    });
}

void Bidapp_removeBanner_platform(const char* bannerId)
{
    NSString* identifier = ToNSString(bannerId);
    if (!identifier)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [banners[identifier] stopAutorefresh];
        [banners[identifier] removeFromSuperview];
        [banners removeObjectForKey:identifier];
    });
}

void Bidapp_setGDPRConsent_platform(bool consent)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BIDConsent.GDPR = @(consent);
    });
}

void Bidapp_setCCPAConsent_platform(bool consent)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BIDConsent.CCPA = @(consent);
    });
}

void Bidapp_setSubjectToCOPPA_platform(bool subjectToCOPPA)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        BIDConsent.COPPA = @(!subjectToCOPPA);
    });
}

void Bidapp_requestTrackingAuthorization_platform()
{
    if (![NSThread isMainThread])
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            
            [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus s){}];
        });
        
        return;
    }
    
    [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus s){}];
}

bool Bidapp_hasInterstitial(const char* idt)
{
    if (![NSThread isMainThread])
    {
        return false;
    }
    
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return false;
    }
    
    return [interstitials[identifier] isAdReady];
}

bool Bidapp_hasRewarded(const char* idt)
{
    if (![NSThread isMainThread])
    {
        return false;
    }
    
    NSString* identifier = ToNSString(idt);
    if (!identifier)
    {
        return false;
    }
    
    return [rewardeds[identifier] isAdReady];
}

void Bidapp_pause_platform(bool pauseStatus)
{
    UnrealPause_bidapp(pauseStatus ? 1 : 0);
}

void Bidapp_setParameterValue_platform(const char* parameterName, const char* parameterValue, const char* instanceIdentifier)
{
}

const char* Bidapp_getParameterValue_platform(const char* parameterName, const char* instanceIdentifier)
{
    return FromNSString(@"");
}

#endif
