// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class GamsLibraryLibrary : ModuleRules
{
	public GamsLibraryLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

    PublicDependencyModuleNames.AddRange(
      new string[]
      {
        "MadaraLibraryLibrary"
				// ... add other public dependencies that you statically link with here ...
			}
      );

    PublicIncludePaths.AddRange(
      new string[] {
              "ThirdParty",
      });

    PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    //PublicDefinitions.Add("MADARA_BUILD_STATIC=1");
    //PublicDefinitions.Add("GAMS_BUILD_STATIC=1");
    PublicDefinitions.Add("_USE_MATH_DEFINES=1");
    PublicDefinitions.Add("BOOST_ALL_NO_LIB=1");
    PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS=1");

    bUseRTTI = true;
    bEnableExceptions = true;

    string BaseLibDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", "..", ".."));
    string MadaraLibDirectory = Path.Combine(BaseLibDirectory, "ThirdParty",
      "gams", Target.Platform.ToString());

    if (Target.Platform == UnrealTargetPlatform.Win64)
    {
      RuntimeDependencies.Add(Path.Combine(MadaraLibDirectory, "GAMS.dll"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Mac)
    {
    }
  }
}
