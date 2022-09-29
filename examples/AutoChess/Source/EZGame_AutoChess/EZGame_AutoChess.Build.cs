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
        
        PublicDefinitions.Add("ASIO_STANDALONE");
        PublicDefinitions.Add("_WEBSOCKETPP_CPP11_INTERNAL_");
        PublicDefinitions.Add("ASIO_HAS_STD_TYPE_TRAITS");
        PublicDefinitions.Add("ASIO_HAS_STD_SHARED_PTR");
        PublicDefinitions.Add("ASIO_HAS_STD_ADDRESSOF");
        PublicDefinitions.Add("ASIO_HAS_STD_ATOMIC");
        PublicDefinitions.Add("ASIO_HAS_STD_CHRONO");
        PublicDefinitions.Add("ASIO_HAS_CSTDINT");
        PublicDefinitions.Add("ASIO_HAS_STD_ARRAY");
        PublicDefinitions.Add("ASIO_HAS_STD_SYSTEM_ERROR");

        PrivateIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        PrivateIncludePaths.Add(Path.Combine(ThirdPartPath, "inc/asio"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartPath, "inc/asio"));
        
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
