// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class GamsAgents : ModuleRules
{
  private string ModulePath
  {
    get { return ModuleDirectory; }
  }

  private string ThirdPartyPath
  {
    get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/Win64")); }
  }

  public GamsAgents(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
        "GamsLibrary"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
        "GamsLibrary"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);



    // Boost has lots of warnings. Disable them
    PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    bUseRTTI = true;
    bEnableExceptions = true;

    // @Alex: The following mostly uncommented got me the furthest in VS
    //        compilation. Preference would still probably be that GamsAgent
    //        is a base for anyone to subclass their agents in Unreal and
    //        Agent simply depends on GamsLibrary to setup the linking and
    //        includes and what not. I am totally fine with just copying
    //        everything to local directories and not referencing anything
    //        in MADARA_ROOT/GAMS_ROOT. I was just seeing if I could get
    //        the environment variable stuff working first.

    //string CAPNP_ROOT =
    //  Environment.GetEnvironmentVariable("CAPNP_ROOT");
    //string EIGEN_ROOT =
    //  Environment.GetEnvironmentVariable("EIGEN_ROOT");
    //string BOOST_ROOT =
    //  Environment.GetEnvironmentVariable("BOOST_ROOT");
    //string MADARA_ROOT =
    //  Environment.GetEnvironmentVariable("MADARA_ROOT");
    //string GAMS_ROOT =
    //  Environment.GetEnvironmentVariable("GAMS_ROOT");

    //// add universal library paths
    //PublicLibraryPaths.Add(Path.Combine(MADARA_ROOT, "lib"));
    //PublicLibraryPaths.Add(Path.Combine(GAMS_ROOT, "lib"));

    //// add universal include paths
    //PublicIncludePaths.Add(Path.Combine(MADARA_ROOT, "include"));
    //PublicIncludePaths.Add(Path.Combine(GAMS_ROOT, "src"));
    //PublicIncludePaths.Add(EIGEN_ROOT);

    //PrivateIncludePaths.Add(Path.Combine(MADARA_ROOT, "include"));
    //PrivateIncludePaths.Add(Path.Combine(GAMS_ROOT, "src"));
    //PrivateIncludePaths.Add(EIGEN_ROOT);

    //string GamsLibPath = Path.Combine(ThirdPartyPath, "Gams", "Libraries");

    //// add universal libraries
    ////PublicAdditionalLibraries.Add("MADARA.lib");
    ////PublicAdditionalLibraries.Add("GAMS.lib");
    //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "GAMS.lib"));
    //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "MADARA.lib"));
    //PublicDelayLoadDLLs.Add(Path.Combine(ThirdPartyPath, "GAMS.dll"));
    //PublicDelayLoadDLLs.Add(Path.Combine(ThirdPartyPath, "MADARA.dll"));
    ////PublicAdditionalLibraries.Add("MADARAs.lib");
    ////PublicAdditionalLibraries.Add("GAMSs.lib");

    //// Windows specific build information
    //if (Target.Platform == UnrealTargetPlatform.Win32 ||
    //    Target.Platform == UnrealTargetPlatform.Win64 ||
    //    Target.Platform == UnrealTargetPlatform.XboxOne)
    //{
    //  // try to intelligently build the BOOST references for Windows
    //  string BOOST_VERSION =
    //    Environment.GetEnvironmentVariable("BOOST_VERSION");
    //  string BOOST_STATIC_LIB_PREFIX =
    //    Environment.GetEnvironmentVariable("BOOST_STATIC_LIB_PREFIX");
    //  string BOOST_ARCH =
    //    Environment.GetEnvironmentVariable("BOOST_ARCH");
    //  string BOOST_TOOLSET =
    //    Environment.GetEnvironmentVariable("BOOST_TOOLSET");

    //  // if user hasn't defined their environment, try reasonable defaults
    //  if (BOOST_VERSION == null)
    //  {
    //    BOOST_VERSION = "1_70";
    //  }
    //  if (BOOST_STATIC_LIB_PREFIX == null)
    //  {
    //    BOOST_STATIC_LIB_PREFIX = "lib";
    //  }
    //  if (BOOST_ARCH == null)
    //  {
    //    BOOST_ARCH = "x64";
    //  }
    //  if (BOOST_TOOLSET == null)
    //  {
    //    BOOST_TOOLSET = "vc141";
    //  }

    //  // build the boost system lib name for Debug and Release
    //  string BOOST_SYSTEM_DEBUG = BOOST_STATIC_LIB_PREFIX + "boost_system-" +
    //    BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
    //  string BOOST_SYSTEM_RELEASE = BOOST_STATIC_LIB_PREFIX + "boost_system-" +
    //    BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
    //  string BOOST_FS_DEBUG = BOOST_STATIC_LIB_PREFIX + "boost_filesystem-" +
    //    BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";
    //  string BOOST_FS_RELEASE = BOOST_STATIC_LIB_PREFIX + "boost_filesystem-" +
    //    BOOST_TOOLSET + "-mt-gd-" + BOOST_ARCH + "-" + BOOST_VERSION + ".lib";

    //  //// add the Boost lib path
    //  PublicLibraryPaths.Add(Path.Combine(BOOST_ROOT, "stage", "lib"));

    //  // depending on if we're in Debug or Release, the user needs diff lib
    //  if (Target.Configuration == UnrealTargetConfiguration.Debug)
    //  {
    //    PublicAdditionalLibraries.Add(BOOST_SYSTEM_DEBUG);
    //    PublicAdditionalLibraries.Add(BOOST_FS_DEBUG);
    //  }
    //  else
    //  {
    //    PublicAdditionalLibraries.Add(BOOST_SYSTEM_RELEASE);
    //    PublicAdditionalLibraries.Add(BOOST_FS_RELEASE);
    //  }

    //  PublicIncludePaths.Add(BOOST_ROOT);


    //  Console.WriteLine("... PrivateIncludePaths -> \n" + string.Join("\n  ", PrivateIncludePaths));
    //  Console.WriteLine("... PublicIncludePaths -> \n" + string.Join("\n  ", PublicIncludePaths));

    //  Console.WriteLine("... PublicLibraryPaths -> \n" + string.Join("\n  ", PublicLibraryPaths));

    //  Console.WriteLine("... PublicAdditionalLibraries -> \n" + string.Join("\n  ", PublicAdditionalLibraries));
    //  Console.WriteLine("... ThirdPartyPath -> " + ThirdPartyPath);
    //}
    //// Linux or Mac specific build information
    //else if (Target.Platform == UnrealTargetPlatform.Linux ||
    //         Target.Platform == UnrealTargetPlatform.Mac)
    //{
    //  PublicAdditionalLibraries.Add("boost_system");
    //}

    //// MADARA and BOOST need RTTI
    //bUseRTTI = true;
    //bEnableExceptions = true;

    // @Alex: note that some of these refer to things you probably don't want
    //        e.g., STATIC. On Windows, building MADARA and GAMS creates a
    //        library that is multiple GBs in size. I believe this is because
    //        of BOOST being incorporated into the .lib. The dlls, by contrast,
    //        are just 2MB for MADARA and 1.1MB for GAMS on Release 64 bit.

    //// Boost has lots of warnings. Disable them
    //PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
    //PublicDefinitions.Add("MADARA_NO_THREAD_LOCAL=1");
    //PublicDefinitions.Add("MADARA_BUILD_STATIC=1");
    //PublicDefinitions.Add("GAMS_BUILD_STATIC=1");
    //PublicDefinitions.Add("_USE_MATH_DEFINES=1");
    //PublicDefinitions.Add("BOOST_ALL_NO_LIB=1");
    //PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS=1");
    //PublicDefinitions.Add("BOOST_FILESYSTEM_NO_LIB=1");
    //PublicDefinitions.Add("BOOST_DATE_TIME_NO_LIB=1");
  }
}
