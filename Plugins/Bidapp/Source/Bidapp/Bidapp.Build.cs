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
		var BidappIOSPath = Path.Combine( ModuleDirectory, "..", "ThirdParty", "IOS" );
		var BidappSDKPath = Path.Combine( BidappIOSPath, "bidapp.embeddedframework.zip" );
		var BidappAppLovinSDKPath = Path.Combine( BidappIOSPath, "adapters", "AppLovinSDK.embeddedframework.zip" );
        	var BidappAppLovinAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappApplovinAdapter.embeddedframework.zip" );
        	var BidappAppLovinMaxAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappApplovinMaxAdapter.embeddedframework.zip" );

		var BidappAdMobSDKPath = Path.Combine( BidappIOSPath, "adapters", "GoogleMobileAds.embeddedframework.zip" );
		var BidappAdMobAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappAdmobAdapter.embeddedframework.zip" );
		var BidappAdMobSDKDep0Path = Path.Combine( BidappIOSPath, "adapters", "GoogleAppMeasurement.embeddedframework.zip" );
		var BidappAdMobSDKDep1Path = Path.Combine( BidappIOSPath, "adapters", "GoogleAppMeasurementIdentitySupport.embeddedframework.zip" );
		var BidappAdMobSDKDep2Path = Path.Combine( BidappIOSPath, "adapters", "GoogleUtilities.embeddedframework.zip" );
		var BidappAdMobSDKDep3Path = Path.Combine( BidappIOSPath, "adapters", "nanopb.embeddedframework.zip" );
		var BidappAdMobSDKDep4Path = Path.Combine( BidappIOSPath, "adapters", "FBLPromises.embeddedframework.zip" );
		var BidappAdMobSDKDep5Path = Path.Combine( BidappIOSPath, "adapters", "UserMessagingPlatform.embeddedframework.zip" );


		var BidappVungleAdsSDKPath = Path.Combine( BidappIOSPath, "adapters", "VungleAdsSDK.embeddedframework.zip" );
		var BidappLiftoffAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappLiftoffAdapter.embeddedframework.zip" );

		var BidappUnitySDKPath = Path.Combine( BidappIOSPath, "adapters", "UnityAds.embeddedframework.zip" );
		var BidappUnityAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappUnityAdapter.embeddedframework.zip" );

		var BidappFacebookSDKPath = Path.Combine( BidappIOSPath, "adapters", "FBAudienceNetwork.embeddedframework.zip" );
		var BidappFacebookAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappFacebookAdapter.embeddedframework.zip" );

		var BidappChartboostSDKPath = Path.Combine( BidappIOSPath, "adapters", "ChartboostSDK.embeddedframework.zip" );
		var BidappChartboostAdapterPath = Path.Combine( BidappIOSPath, "adapters", "BidappChartboostAdapter.embeddedframework.zip" );

		var BidappPodsPath = Path.Combine( BidappIOSPath, "Pods" );

		if ( !File.Exists( BidappSDKPath ) || !File.Exists( BidappAppLovinSDKPath ) || !File.Exists( BidappAppLovinAdapterPath ) || !File.Exists( BidappAppLovinMaxAdapterPath ) ||
!File.Exists( BidappAdMobSDKPath ) || !File.Exists( BidappAdMobAdapterPath ) || !File.Exists( BidappAdMobSDKDep0Path ) || !File.Exists( BidappAdMobSDKDep1Path ) || !File.Exists( BidappAdMobSDKDep2Path ) || !File.Exists( BidappAdMobSDKDep3Path ) || !File.Exists( BidappAdMobSDKDep4Path ) || !File.Exists( BidappAdMobSDKDep5Path ) || !File.Exists( BidappVungleAdsSDKPath ) || !File.Exists( BidappLiftoffAdapterPath ) || !File.Exists( BidappUnitySDKPath ) || !File.Exists( BidappUnityAdapterPath ) || !File.Exists( BidappFacebookSDKPath ) || !File.Exists( BidappFacebookAdapterPath ) || !File.Exists( BidappChartboostSDKPath ) || !File.Exists( BidappChartboostAdapterPath ))
		{
			System.Console.WriteLine( "Bidapp IOS Plugin not found" );
			PublicDefinitions.Add( "WITH_Bidapp=0" );
			return;
		}

		System.Console.WriteLine( "Bidapp IOS Plugin found" );

		var PluginPath = Utils.MakePathRelativeTo( ModuleDirectory, Target.RelativeEnginePath );
		AdditionalPropertiesForReceipt.Add( "IOSPlugin", Path.Combine( PluginPath, "Bidapp_UPL_IOS.xml" ) );

		bEnableObjCAutomaticReferenceCounting = true;

		// Add support for linking with Swift frameworks
		PrivateDependencyModuleNames.Add( "Swift" );

		// Add the bidapp framework
		PublicAdditionalFrameworks.Add(
			new Framework(
				"bidapp",
				BidappSDKPath
			)
		);

		// Add the AppLovin SDK framework
		PublicAdditionalFrameworks.Add(
			new Framework(
				"AppLovinSDK",
				BidappAppLovinSDKPath,
				"Resources/AppLovinSDKResources.bundle"
			)
		);

		// Add the AppLovin adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappApplovinAdapter",
				BidappAppLovinAdapterPath
			)
		);

		// Add the AppLovin Max adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappApplovinMaxAdapter",
				BidappAppLovinMaxAdapterPath
			)
		);

		// Add the AdMob adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappAdmobAdapter",
				BidappAdMobAdapterPath
			)
		);

		// Add the AdMob SDK
		PublicAdditionalFrameworks.Add(
			new Framework(
				"GoogleMobileAds",
				BidappAdMobSDKPath
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"GoogleAppMeasurement",
				BidappAdMobSDKDep0Path
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"GoogleAppMeasurementIdentitySupport",
				BidappAdMobSDKDep1Path
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"GoogleUtilities",
				BidappAdMobSDKDep2Path
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"nanopb",
				BidappAdMobSDKDep3Path
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"FBLPromises",
				BidappAdMobSDKDep4Path
			)
		);

		// Add the AdMob SDK dependency
		PublicAdditionalFrameworks.Add(
			new Framework(
				"UserMessagingPlatform",
				BidappAdMobSDKDep5Path
			)
		);

		// Add the Liftoff adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappLiftoffAdapter",
				BidappLiftoffAdapterPath
			)
		);

		// Add the VungleAds SDK
		PublicAdditionalFrameworks.Add(
			new Framework(
				"VungleAdsSDK",
				BidappVungleAdsSDKPath
			)
		);

		// Add the Unity adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappUnityAdapter",
				BidappUnityAdapterPath
			)
		);

		// Add the VungleAds SDK
		PublicAdditionalFrameworks.Add(
			new Framework(
				"UnityAds",
				BidappUnitySDKPath
			)
		);


		// Add the Facebook adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappFacebookAdapter",
				BidappFacebookAdapterPath
			)
		);

		// Add the Facebook SDK
		PublicAdditionalFrameworks.Add(
			new Framework(
				"FBAudienceNetwork",
				BidappFacebookSDKPath
			)
		);

		// Add the Chartboost adapter
		PublicAdditionalFrameworks.Add(
			new Framework(
				"BidappChartboostAdapter",
				BidappChartboostAdapterPath
			)
		);

		// Add the Chartboost SDK
		PublicAdditionalFrameworks.Add(
			new Framework(
				"ChartboostSDK",
				BidappChartboostSDKPath
			)
		);

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

		// Parse installed Pods configuration
		if ( Directory.Exists( BidappPodsPath ) )
		{
			XDocument PodConfig = XDocument.Load( Path.Combine( BidappPodsPath, "config.xml" ) );

			var TagsToPublicSet = new[] {
		        ("PublicFrameworks", PluginFrameworks),
		        ("PublicWeakFrameworks", PluginWeakFrameworks),
		        ("PublicSystemLibraries", PluginSystemLibraries)
		    };

		    foreach ( var (Tag, PluginSet) in TagsToPublicSet )
		    {
		        foreach ( var Item in PodConfig.Descendants( Tag ).Elements( "Item" ) )
		        {
		        	PluginSet.Add( Item.Value );
		        }
		    }

			// Process the additional libraries
			foreach ( var Item in PodConfig.Descendants( "PublicAdditionalLibraries" ).Elements( "Item" ) )
			{
				var LibraryPath = Path.Combine( BidappPodsPath, Item.Value );

				System.Console.WriteLine( "Adding CocoaPod library: " + LibraryPath );

				PublicAdditionalLibraries.Add( LibraryPath );
			}

			// Process the additional frameworks
			foreach ( var Item in PodConfig.Descendants( "PublicAdditionalFrameworks" ).Elements( "Item" ) )
			{
                var Name = Item.Element( "Name" ).Value;
				var FrameworkPath = Path.Combine( BidappPodsPath, Item.Element( "Path" ).Value );
				var Resources = Path.Combine( BidappPodsPath, Item.Element( "Resources" ).Value );

				System.Console.WriteLine( "Adding CocoaPod framework: " + Name + " (" + FrameworkPath + ")" );

				var AdditionalFramework = Resources != "None"
		            ? new Framework( Name, FrameworkPath, Resources )
		            : new Framework( Name, FrameworkPath );

				PublicAdditionalFrameworks.Add( AdditionalFramework );
			}
		}

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
