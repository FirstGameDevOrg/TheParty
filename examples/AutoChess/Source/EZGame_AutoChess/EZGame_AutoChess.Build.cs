// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using System;


public class EZGame_AutoChess : ModuleRules
{
    private string ThirdPartPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, @"..\ThirdParty"));
        }
    }

    public EZGame_AutoChess(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        PrivateIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        
        PublicDependencyModuleNames.Add("Protobuf");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","ProceduralMeshComponent", "AIModule", "GameplayTasks", "WebSockets", "Json" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
