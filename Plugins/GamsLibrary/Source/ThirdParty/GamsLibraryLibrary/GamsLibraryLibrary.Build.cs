// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class GamsLibraryLibrary : ModuleRules
{
	public GamsLibraryLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
	Type = ModuleType.External;

    string CAPNP_ROOT =
      Environment.GetEnvironmentVariable("CAPNP_ROOT");
    string EIGEN_ROOT =
      Environment.GetEnvironmentVariable("EIGEN_ROOT");
    string BOOST_ROOT =
      Environment.GetEnvironmentVariable("BOOST_ROOT");
    string MADARA_ROOT =
      Environment.GetEnvironmentVariable("MADARA_ROOT");
    string GAMS_ROOT =
      Environment.GetEnvironmentVariable("GAMS_ROOT");

    // add universal library paths
    PublicLibraryPaths.Add(Path.Combine(MADARA_ROOT, "lib"));
    PublicLibraryPaths.Add(Path.Combine(GAMS_ROOT, "lib"));

    // add universal include paths
    PublicIncludePaths.Add(Path.Combine(MADARA_ROOT, "include"));
    PublicIncludePaths.Add(Path.Combine(GAMS_ROOT, "src"));
    PublicIncludePaths.Add(EIGEN_ROOT);

    PrivateIncludePaths.Add(Path.Combine(MADARA_ROOT, "include"));
    PrivateIncludePaths.Add(Path.Combine(GAMS_ROOT, "src"));
    PrivateIncludePaths.Add(EIGEN_ROOT);

    // add universal libraries
    PublicDelayLoadDLLs.Add("MADARA");
    PublicDelayLoadDLLs.Add("GAMS");

    // Windows specific build information
    if (Target.Platform == UnrealTargetPlatform.Win32 ||
        Target.Platform == UnrealTargetPlatform.Win64 ||
        Target.Platform == UnrealTargetPlatform.XboxOne)
    {
      // try to intelligently build the BOOST references for Windows
      string BOOST_VERSION =
        Environment.GetEnvironmentVariable("BOOST_VERSION");
      string BOOST_STATIC_LIB_PREFIX =
        Environment.GetEnvironmentVariable("BOOST_STATIC_LIB_PREFIX");
      string BOOST_ARCH =
        Environment.GetEnvironmentVariable("BOOST_ARCH");
      string BOOST_TOOLSET =
        Environment.GetEnvironmentVariable("BOOST_TOOLSET");

      // if user hasn't defined their environment, try reasonable defaults
      if (BOOST_VERSION == null)
      {
        BOOST_VERSION = "1_70";
      }
      if (BOOST_STATIC_LIB_PREFIX == null)
      {
        BOOST_STATIC_LIB_PREFIX = "lib";
      }
      if (BOOST_ARCH == null)
      {
        BOOST_ARCH = "x64";
      }
      if (BOOST_TOOLSET == null)
      {
        BOOST_TOOLSET = "vc141";
      }

      // build the boost system lib name for Debug and Release
      string BOOST_SYSTEM_DEBUG = BOOST_STATIC_LIB_PREFIX + "boost_system-" +
        BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
      string BOOST_SYSTEM_RELEASE = BOOST_STATIC_LIB_PREFIX + "boost_system-" +
        BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
      string BOOST_FS_DEBUG = BOOST_STATIC_LIB_PREFIX + "boost_filesystem-" +
        BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
      string BOOST_FS_RELEASE = BOOST_STATIC_LIB_PREFIX + "boost_filesystem-" +
        BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";

      // add the Boost lib path
      PublicLibraryPaths.Add(Path.Combine(BOOST_ROOT, "stage", "lib"));

      // depending on if we're in Debug or Release, the user needs diff lib
      if (Target.Configuration == UnrealTargetConfiguration.Debug)
      {
        PublicAdditionalLibraries.Add(BOOST_SYSTEM_DEBUG);
        PublicAdditionalLibraries.Add(BOOST_FS_DEBUG);
      }
      else
      {
        PublicAdditionalLibraries.Add(BOOST_SYSTEM_RELEASE);
        PublicAdditionalLibraries.Add(BOOST_FS_RELEASE);
      }

      PublicIncludePaths.Add(BOOST_ROOT);
    }
    // Linux or Mac specific build information
    else if (Target.Platform == UnrealTargetPlatform.Linux ||
             Target.Platform == UnrealTargetPlatform.Mac)
    {
      PublicAdditionalLibraries.Add("boost_system");
    }

    // MADARA and BOOST need RTTI
    bUseRTTI = true;
    bEnableExceptions = true;

    // Boost has lots of warnings. Disable them
    PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    PublicDefinitions.Add("_USE_MATH_DEFINES=1");
  }
}
