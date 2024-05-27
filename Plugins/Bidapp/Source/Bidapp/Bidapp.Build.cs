/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/

using UnrealBuildTool;
using System.Diagnostics;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;


public class Bidapp : ModuleRules
{
	public Bidapp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/BidappPrivatePCH.h";

		PublicIncludePaths.AddRange( new string[] {} );
		PrivateIncludePaths.AddRange( new string[] { "Bidapp/Private" } );
		PrivateIncludePathModuleNames.AddRange( new string[] { "Settings" } );
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "Json", "JsonUtilities" } );
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Projects"
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange( new string[] { } );
		
	if ( Target.Platform == UnrealTargetPlatform.IOS )
		{
			InstallIOS();
		}
		else if ( Target.Platform == UnrealTargetPlatform.Android )
		{
			InstallAndroid();
		}
		else
		{
			PublicDefinitions.Add( "WITH_Bidapp=0" );
		}
	}

	private void InstallIOS()
	{
        var PluginPath = Utils.MakePathRelativeTo( ModuleDirectory, Target.RelativeEnginePath );
        AdditionalPropertiesForReceipt.Add( "IOSPlugin", Path.Combine( PluginPath, "Bidapp_UPL_IOS.xml" ) );

        bEnableObjCAutomaticReferenceCounting = true;

        // Add support for linking with Swift frameworks
        PrivateDependencyModuleNames.Add( "Swift" );
 
		var BidappIOSPath = Path.Combine( ModuleDirectory, "..", "ThirdParty", "IOS" );
        
		var BidappSDKPath = Path.Combine( BidappIOSPath, "bidapp.embeddedframework.zip" );
        if ( File.Exists( BidappSDKPath ) )
        {
            System.Console.WriteLine( "Bidapp IOS SDK found" );
            // Add the bidapp framework
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "bidapp",
                    BidappSDKPath,
                    "Resources/bidapp.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp IOS Plugin not found" );
            PublicDefinitions.Add( "WITH_Bidapp=0" );
            return;
        }
        
		var BidappAppLovinSDKPath = Path.Combine( BidappIOSPath, "networks", "AppLovinSDK.embeddedframework.zip" );
        if ( File.Exists( BidappAppLovinSDKPath ) )
        {
            System.Console.WriteLine( "AppLovin IOS SDK found" );
            // Add the AppLovin SDK framework
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "AppLovinSDK",
                    BidappAppLovinSDKPath,
                    "Resources/AppLovinSDKResources.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "AppLovin IOS SDK NOT found" );
        }
  
        var BidappAppLovinAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappApplovinAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappAppLovinAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp AppLovin adapter IOS found" );
            // Add the AppLovin adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappApplovinAdapter",
                    BidappAppLovinAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp AppLovin adapter IOS NOT found" );
        }
        
        var BidappAppLovinMaxAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappApplovinMaxAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappAppLovinMaxAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp AppLovin MAX adapter IOS found" );
            // Add the AppLovin Max adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappApplovinMaxAdapter",
                    BidappAppLovinMaxAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp AppLovin MAX adapter IOS NOT found" );
        }

		var BidappAdMobSDKPath = Path.Combine( BidappIOSPath, "networks", "GoogleMobileAds.embeddedframework.zip" );
        if ( File.Exists( BidappAdMobSDKPath ) )
        {
            System.Console.WriteLine( "AdMob IOS SDK found" );
            // Add the AdMob SDK
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "GoogleMobileAds",
                    BidappAdMobSDKPath,
                    "Resources/GoogleMobileAdsResources.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "AdMob IOS SDK NOT found" );
        }
  
		var BidappAdMobAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappAdmobAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappAdMobAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp AdMob adapter IOS found" );
            // Add the AdMob adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappAdmobAdapter",
                    BidappAdMobAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp AdMob adapter IOS NOT found" );
        }

		var BidappVungleAdsSDKPath = Path.Combine( BidappIOSPath, "networks", "VungleAdsSDK.embeddedframework.zip" );
        if ( File.Exists( BidappVungleAdsSDKPath ) )
        {
            System.Console.WriteLine( "VungleAds IOS SDK found" );
            // Add the VungleAds SDK
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "VungleAdsSDK",
                    BidappVungleAdsSDKPath,
                    "Resources/VungleAds.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "VungleAds IOS SDK  NOT found" );
        }
        
		var BidappLiftoffAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappLiftoffAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappLiftoffAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp Liftoff adapter IOS found" );
            // Add the Liftoff adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappLiftoffAdapter",
                    BidappLiftoffAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp Liftoff adapter IOS NOT found" );
        }

		var BidappUnitySDKPath = Path.Combine( BidappIOSPath, "networks", "UnityAds.embeddedframework.zip" );
        if ( File.Exists( BidappUnitySDKPath ) )
        {
            System.Console.WriteLine( "UnityAds IOS SDK found" );
            // Add the UnityAds SDK
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "UnityAds",
                    BidappUnitySDKPath,
                    "Resources/UnityAdsResources.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "UnityAds IOS SDK NOT found" );
        }
  
		var BidappUnityAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappUnityAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappUnityAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp Unity adapter IOS found" );
            // Add the Unity adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappUnityAdapter",
                    BidappUnityAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp Unity adapter IOS NOT found" );
        }

		var BidappFacebookSDKPath = Path.Combine( BidappIOSPath, "networks", "FBAudienceNetwork.embeddedframework.zip" );
        if ( File.Exists( BidappFacebookSDKPath ) )
        {
            System.Console.WriteLine( "Facebook Audience Network IOS SDK found" );
            // Add the Facebook SDK
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "FBAudienceNetwork",
                    BidappFacebookSDKPath,
                    "Resources/FBAudienceNetwork.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Facebook Audience Network IOS SDK NOT found" );
        }
  
		var BidappFacebookAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappFacebookAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappFacebookAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp Facebook adapter IOS found" );
            // Add the Facebook adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappFacebookAdapter",
                    BidappFacebookAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp Facebook adapter IOS NOT found" );
        }

		var BidappChartboostSDKPath = Path.Combine( BidappIOSPath, "networks", "ChartboostSDK.embeddedframework.zip" );
        if ( File.Exists( BidappChartboostSDKPath ) )
        {
            System.Console.WriteLine( "Chartboost IOS SDK found" );
            // Add the Chartboost SDK
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "ChartboostSDK",
                    BidappChartboostSDKPath,
                    "Resources/ChartboostSDKResources.bundle"
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Chartboost IOS SDK NOT found" );
        }
        
		var BidappChartboostAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappChartboostAdapter.embeddedframework.zip" );
        if ( File.Exists( BidappChartboostAdapterPath ) )
        {
            System.Console.WriteLine( "Bidapp Chartboost adapter IOS found" );
            // Add the Chartboost adapter
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "BidappChartboostAdapter",
                    BidappChartboostAdapterPath
                )
            );
        }
        else
        {
            System.Console.WriteLine( "Bidapp Chartboost adapter IOS NOT found" );
        }

		var PluginFrameworks = new HashSet<string> {
			"AdSupport",
			"AudioToolbox",
			"AVFoundation",
			"CFNetwork",
			"CoreGraphics",
			"CoreMedia",
			"CoreMotion",
			"CoreTelephony",
			"MessageUI",
			"SafariServices",
			"StoreKit",
			"SystemConfiguration",
			"UIKit",
			"WebKit"
		};

		var PluginWeakFrameworks = new HashSet<string> { "AppTrackingTransparency" };
		var PluginSystemLibraries = new HashSet<string>();

		PublicFrameworks.AddRange( PluginFrameworks.ToArray() );
		PublicWeakFrameworks.AddRange( PluginWeakFrameworks.ToArray() );
		PublicSystemLibraries.AddRange( PluginSystemLibraries.ToArray() );

		AddEngineThirdPartyPrivateStaticDependencies( Target, "zlib" );

		PublicDefinitions.Add( "WITH_Bidapp=1" );
	}
    
	private void InstallAndroid()
	{
		var BidappAndroidPath = Path.Combine( ModuleDirectory, "..", "ThirdParty", "Android", "repository", "io", "bidapp", "bidapp-unreal-plugin", "release" );
		var BidappPluginPath = Path.Combine( BidappAndroidPath, "bidapp-unreal-plugin-release.aar" );
		if ( !File.Exists( BidappPluginPath ) )
		{
			System.Console.WriteLine( "Bidapp Android Plugin not found" );
			PublicDefinitions.Add( "WITH_Bidapp=0" );
			return;
		}

		System.Console.WriteLine( "Bidapp Android Plugin found" );

		PrivateIncludePaths.Add( "Bidapp/Private/Android" );
		
		// Includes Android JNI support 
		PrivateDependencyModuleNames.Add( "Launch" );

		var PluginPath = Utils.MakePathRelativeTo( ModuleDirectory, Target.RelativeEnginePath );
		AdditionalPropertiesForReceipt.Add( "AndroidPlugin", Path.Combine( PluginPath, "Bidapp_UPL_Android.xml" ) );

		PublicDefinitions.Add( "WITH_Bidapp=1" );
	}
}
