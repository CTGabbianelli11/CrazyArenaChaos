// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrazyArenaChaos : ModuleRules
{
	public CrazyArenaChaos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HairStrandsCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });

    }
}
