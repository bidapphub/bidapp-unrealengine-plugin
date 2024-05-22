using UnrealBuildTool;

public class BidappDemo : ModuleRules
{
	public BidappDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Bidapp" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
