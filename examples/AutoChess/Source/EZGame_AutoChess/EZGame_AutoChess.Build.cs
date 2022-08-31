// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using System;


public class EZGame_AutoChess : ModuleRules
{
    private string ThirdPartPath
    {
        //通过get方法获得ModuleDirectory上两级的ThirdPart目录。
        //ModuleDirectory指的是Build.cs所在的目录
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "lib"));
        }
    }

    public EZGame_AutoChess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		string libBaseDir = Path.Combine(ThirdPartPath, Target.Platform == UnrealTargetPlatform.Win64 ? "x64" : "x86");

        //PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotobuf.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotoc.lib"));

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","ProceduralMeshComponent", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
