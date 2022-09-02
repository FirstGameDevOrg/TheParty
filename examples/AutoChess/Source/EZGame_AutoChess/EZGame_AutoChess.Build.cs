// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using System;


public class EZGame_AutoChess : ModuleRules
{
    private string ThirdPartPath
    {
        //ͨ��get�������ModuleDirectory��������ThirdPartĿ¼��
        //ModuleDirectoryָ����Build.cs���ڵ�Ŀ¼
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, @"..\ThirdParty"));
        }
    }

    public EZGame_AutoChess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        Definitions.Add("ASIO_STANDALONE");
        Definitions.Add("_WEBSOCKETPP_CPP11_INTERNAL_");
        Definitions.Add("ASIO_HAS_STD_TYPE_TRAITS");
        Definitions.Add("ASIO_HAS_STD_SHARED_PTR");
        Definitions.Add("ASIO_HAS_STD_ADDRESSOF");
        Definitions.Add("ASIO_HAS_STD_ATOMIC");
        Definitions.Add("ASIO_HAS_STD_CHRONO");
        Definitions.Add("ASIO_HAS_CSTDINT");
        Definitions.Add("ASIO_HAS_STD_ARRAY");
        Definitions.Add("ASIO_HAS_STD_SYSTEM_ERROR");
        
        
        string libBaseDir = Path.Combine(ThirdPartPath, "lib", Target.Platform == UnrealTargetPlatform.Win64 ? "x64" : "x86");
        PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotobuf.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotoc.lib"));

        PrivateIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartPath, "inc"));
        PrivateIncludePaths.Add(Path.Combine(ThirdPartPath, "inc/asio"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartPath, "inc/asio"));

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","ProceduralMeshComponent", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
