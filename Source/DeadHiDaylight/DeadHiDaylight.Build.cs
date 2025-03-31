// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeadHiDaylight : ModuleRules
{
	public DeadHiDaylight(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "AnimGraphRuntime", "AnimGraphRuntime" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
		
		PublicIncludePaths.AddRange(new string[] {
			"DeadHiDaylight/Public/InteractableObjects",
			"DeadHiDaylight/Public/InteractableObjects/Generator",
			"DeadHiDaylight/Public/InteractableObjects/Meathook",
			"DeadHiDaylight/Public/InteractableObjects/ExitDoor",
			"DeadHiDaylight/Public/InteractableObjects/Pallet",
			"DeadHiDaylight/Public/Net",
			"DeadHiDaylight/Public/Net/0Global",
			"DeadHiDaylight/Public/Net/1Lobby",
			"DeadHiDaylight/Public/Net/2Campfire",
			"DeadHiDaylight/Public/Net/3Sacrifice",
			"DeadHiDaylight/Public/Slasher",
			"DeadHiDaylight/Public/UI",
		});
	}
}
