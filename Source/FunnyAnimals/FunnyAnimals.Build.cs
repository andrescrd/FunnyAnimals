// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FunnyAnimals : ModuleRules
{
	public FunnyAnimals(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" ,"UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// PrivateDependencyModuleNames.Add("OnlineSubsystemNull");
		// PrivateDependencyModuleNames.Add("OnlineSubsystemUtils");
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");


		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
