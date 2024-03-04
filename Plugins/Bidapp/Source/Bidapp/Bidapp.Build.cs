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
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "Json", "JsonUtilities" } );
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"Core",
				"CoreUObject",
				"Engine",
				"Projects"
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange( new string[] { } );
		

		if ( Target.Platform == UnrealTargetPlatform.Android )
		{
			InstallAndroid();
		}
		else
		{
			PublicDefinitions.Add( "WITH_Bidapp=0" );
		}
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
