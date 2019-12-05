// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class MadaraLibraryLibrary : ModuleRules
{
  public MadaraLibraryLibrary(ReadOnlyTargetRules Target) : base(Target)
  {
    Type = ModuleType.External;

    PublicIncludePaths.AddRange(
      new string[] {
              "ThirdParty",
      });

    PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    PublicDefinitions.Add("MADARA_BUILD_STATIC=1");
    PublicDefinitions.Add("GAMS_BUILD_STATIC=1");
    PublicDefinitions.Add("_USE_MATH_DEFINES=1");
    PublicDefinitions.Add("BOOST_ALL_NO_LIB=1");
    PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS=1");

    bUseRTTI = true;
    bEnableExceptions = true;

    string BaseLibDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", ".."));
    string MadaraLibDirectory = Path.Combine(BaseLibDirectory, "ThirdParty",
      "madara", Target.Platform.ToString());

    if (Target.Platform == UnrealTargetPlatform.Win64)
    {
      RuntimeDependencies.Add(Path.Combine(MadaraLibDirectory, "MADARA.dll"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Mac)
    {
    }
  }
}
