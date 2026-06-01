// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OIbstacleAssault : ModuleRules
{
	public OIbstacleAssault(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"OIbstacleAssault",
			"OIbstacleAssault/Variant_Platforming",
			"OIbstacleAssault/Variant_Platforming/Animation",
			"OIbstacleAssault/Variant_Combat",
			"OIbstacleAssault/Variant_Combat/AI",
			"OIbstacleAssault/Variant_Combat/Animation",
			"OIbstacleAssault/Variant_Combat/Gameplay",
			"OIbstacleAssault/Variant_Combat/Interfaces",
			"OIbstacleAssault/Variant_Combat/UI",
			"OIbstacleAssault/Variant_SideScrolling",
			"OIbstacleAssault/Variant_SideScrolling/AI",
			"OIbstacleAssault/Variant_SideScrolling/Gameplay",
			"OIbstacleAssault/Variant_SideScrolling/Interfaces",
			"OIbstacleAssault/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
