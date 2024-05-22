/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


using UnrealBuildTool;

public class BidappSettings : ModuleRules
{
	public BidappSettings(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivatePCHHeaderFile = "Public/BidappSettings.h";
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		//bUsePrecompiled = true;




        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
        }

        PublicIncludePaths.AddRange(
			new string[] {
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);
			

		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Bidapp"				
			}
			);

		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	
	}
}
