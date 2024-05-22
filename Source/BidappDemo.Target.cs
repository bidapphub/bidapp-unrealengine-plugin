// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BidappDemoTarget : TargetRules
{
	public BidappDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

#if UE_5_3_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
#elif UE_5_2_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
#else
		DefaultBuildSettings = BuildSettingsVersion.V2;
#endif

		ExtraModuleNames.AddRange( new string[] { "BidappDemo" } );

	}
}
