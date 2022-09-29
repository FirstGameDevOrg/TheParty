using UnrealBuildTool;
using System.IO;

public class Protobuf : ModuleRules
{
    public Protobuf(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;
        
        PublicDefinitions.Add("PROTOBUF_ENABLE_DEBUG_LOGGING_MAY_LEAK_PII=1");
        PublicDefinitions.Add("PROTOBUF_BUILTIN_ATOMIC=1");
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
            string libBaseDir = Path.Combine(ModuleDirectory, "lib", "x64");
            PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotobuf.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotoc.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
            string libBaseDir = Path.Combine(ModuleDirectory, "lib", "x86");
            PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotobuf.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(libBaseDir, "libprotoc.lib"));
        }

        ShadowVariableWarningLevel = WarningLevel.Off;
        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true;
        PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
        PublicDefinitions.Add("GOOGLE_PROTOBUF_CMAKE_BUILD");
    }
}