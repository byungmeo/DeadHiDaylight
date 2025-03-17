// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeadHiDaylight : ModuleRules
{
	public DeadHiDaylight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		
		PublicIncludePaths.AddRange(new string[] {
			"DeadHiDaylight/Public/InteractableObjects",
			"DeadHiDaylight/Public/InteractableObjects/Generator",
		});
	}
}
