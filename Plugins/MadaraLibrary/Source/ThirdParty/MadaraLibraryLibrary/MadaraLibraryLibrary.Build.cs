// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class MadaraLibraryLibrary : ModuleRules
{
  public MadaraLibraryLibrary(ReadOnlyTargetRules Target) : base(Target)
  {
    Type = ModuleType.External;
    
    string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", "..", "..", "..", ".."));
    string BaseLibDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory,
      "..", "..", ".."));
    string SourceDirectory = Path.Combine(BaseLibDirectory, "Source",
      "ThirdParty");
    string MadaraLibDirectory = Path.Combine(BaseLibDirectory, "ThirdParty",
      "madara", Target.Platform.ToString());

    System.Console.WriteLine("BaseDir = " + BaseDirectory);
    System.Console.WriteLine("Baselibdir = " + BaseLibDirectory);
    System.Console.WriteLine("SourceDirectory = " + SourceDirectory);
    System.Console.WriteLine("MadaraLibDirectory = " + MadaraLibDirectory);


    PublicIncludePaths.AddRange(
      new string[] {
              SourceDirectory,
      });

    //PublicLibraryPaths.Add(MadaraLibDirectory);

    PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    //PublicDefinitions.Add("MADARA_BUILD_STATIC=1");
    //PublicDefinitions.Add("GAMS_BUILD_STATIC=1");
    PublicDefinitions.Add("_USE_MATH_DEFINES=1");
    PublicDefinitions.Add("BOOST_ALL_NO_LIB=1");
    PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS=1");

    bUseRTTI = false;
    bEnableExceptions = true;

    if (Target.Platform == UnrealTargetPlatform.Win64)
    {
      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Win64");

      System.IO.File.Copy(Path.Combine(MadaraLibDirectory, "MADARA.dll"),
        Path.Combine(BinariesDir, "MADARA.dll"), true);

      PublicAdditionalLibraries.Add(Path.Combine(MadaraLibDirectory, "MADARA.lib"));
      PublicDelayLoadDLLs.Add(Path.Combine(MadaraLibDirectory, "MADARA.dll"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Mac)
    {
      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Mac");

      System.IO.File.Copy(Path.Combine(MadaraLibDirectory, "libMADARA.so"),
        Path.Combine(BinariesDir, "libMADARA.so"), true);
      PublicAdditionalLibraries.Add(Path.Combine(MadaraLibDirectory, "libMADARA.so"));
    }
    else if (Target.Platform == UnrealTargetPlatform.Linux)
    {
      string BinariesDir = Path.Combine(BaseDirectory, "Binaries", "Linux");

      System.IO.File.Copy(Path.Combine(MadaraLibDirectory, "libMADARA.so"),
        Path.Combine(BinariesDir, "libMADARA.so"), true);
      PublicAdditionalLibraries.Add(Path.Combine(MadaraLibDirectory, "libMADARA.so"));
    }
  }
}
