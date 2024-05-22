/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#import <UIKit/UIKit.h>

#define BIDAPP_PLUGIN_CALLBACK(method, idt) UnrealSendMessage_bidapp(BIDAPP_##method, @[], idt)
#define BIDAPP_PLUGIN_CALLBACK_ARG(method, idt, arg) UnrealSendMessage_bidapp(BIDAPP_##method, [NSArray arrayWithObjects:arg], idt)
#define BIDAPP_PLUGIN_CALLBACK_ERROR_ARG_ARG_ARG(method, idt, arg1, arg2, arg3) BIDAPP_PLUGIN_CALLBACK_ARG_ARG_ARG(method, idt, arg1, arg2, arg3)
#define BIDAPP_PLUGIN_CALLBACK_ARG_ARG(method, idt, arg1, arg2) UnrealSendMessage_bidapp(BIDAPP_##method, [NSArray arrayWithObjects:arg1, arg2], idt)
#define BIDAPP_PLUGIN_CALLBACK_ARG_ARG_ARG(method, idt, arg1, arg2, arg3) UnrealSendMessage_bidapp(BIDAPP_##method, [NSArray arrayWithObjects:arg1, arg2, arg3], idt)

#define BIDAPPSDK_PLUGIN_NAME (@"unreal")

#ifdef __cplusplus
extern "C"
{
#endif
//extern void UnitySendMessage(const char *, const char *, const char *);
//extern UIViewController *UnityGetGLViewController();
#ifdef __cplusplus
}
#endif

#ifndef BIDAPP_PLUGIN_VIEW_SOURCE
#define BIDAPP_PLUGIN_VIEW_SOURCE [NSClassFromString(@"BIDUnrealPlugin") view]
#endif

#ifndef UNITY_IOS
#define UNITY_IOS 1
#endif
#define BIDAPP_USE_C_EXTERN
