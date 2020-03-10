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

    bUseRTTI = false;
    bEnableExceptions = true;

    string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", "..", "..", "..", ".."));
    string BaseLibDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", "..", ".."));
    string GamsLibDirectory = Path.Combine(BaseLibDirectory, "ThirdParty",
      "gams", Target.Platform.ToString());

    if (Target.Platform == UnrealTargetPlatform.Win64)
    {
      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Win64");

      System.IO.File.Copy(Path.Combine(GamsLibDirectory, "GAMS.dll"),
        Path.Combine(BinariesDir, "GAMS.dll"), true);

      PublicAdditionalLibraries.Add(Path.Combine(GamsLibDirectory, "GAMS.lib"));
      PublicDelayLoadDLLs.Add(Path.Combine(GamsLibDirectory, "GAMS.dll"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Mac)
    {
      PublicAdditionalLibraries.Add(Path.Combine(GamsLibDirectory, "GAMS.dylib"));

      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Mac");

      System.IO.File.Copy(Path.Combine(GamsLibDirectory, "GAMS.dylib"),
        Path.Combine(BinariesDir, "GAMS.dylib"), true);
    }
    else if (Target.Platform == UnrealTargetPlatform.Linux)
    {
      PublicAdditionalLibraries.Add(Path.Combine(GamsLibDirectory, "libGAMS.so"));

      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Linux");
      System.IO.File.Copy(Path.Combine(GamsLibDirectory, "libGAMS.so"),
        Path.Combine(BinariesDir, "libGAMS.so"), true);
    }
  }
}
