// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Solvi : ModuleRules
{
	public Solvi(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
			{ 
				"Core",
				"CoreOnline",
				"CoreUObject",
				"ApplicationCore",
				"Engine",
				"PhysicsCore",
				"GameplayTags",
				"GameplayTasks",
				"GameplayAbilities",
				"AIModule",
				"ModularGameplay",
				"ModularGameplayActors",
				"DataRegistry",
				"ReplicationGraph",
				"GameFeatures",
				"SignificanceManager",
				"Hotfix",
				"Niagara",
				"ControlFlows",
				"PropertyPath",
			});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GASCompanion",
			"InputCore",
			"Slate",
			"SlateCore",
			"RenderCore",
			"CommonGame",
			"UIExtension",
			"CommonUser",
			"DeveloperSettings",
			"EnhancedInput",
			"NetCore",
			"RHI",
			"Projects",
			"Gauntlet",
			"UMG",
			"CommonUI",
			"CommonInput",
			"AudioMixer",
			"NetworkReplayStreaming",
			"ClientPilot",
			"AudioModulation",
			"EngineSettings",
			"DTLSHandlerComponent",
			"GameplayMessageRuntime"
		});
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
		);
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
	}
}
