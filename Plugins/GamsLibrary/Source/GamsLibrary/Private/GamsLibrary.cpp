// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GamsLibrary.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FGamsLibraryModule"

void FGamsLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

  // Get the base directory of this plugin
  const FString BaseDir = IPluginManager::Get ().FindPlugin (
    "GamsLibrary")->GetBaseDir ();
  const FString MadaraDir = FPaths::Combine (
    *BaseDir, TEXT ("ThirdParty"), TEXT ("gams"));

  // Add on the relative location of the third party dll and load it
  FString LibraryPath;
#if PLATFORM_WINDOWS
  LibraryPath = FPaths::Combine (*MadaraDir, TEXT ("Win64"), TEXT ("GAMS.dll"));
#elif PLATFORM_MAC
  LibraryPath = FPaths::Combine (*MadaraDir, TEXT ("Mac"), TEXT ("GAMS.dylib"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ?
    FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

  if (ExampleLibraryHandle)
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("GAMS LOAD SUCCESS", "Found GAMS library"));
  }
  else
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("GAMS LOAD FAILURE", "Couldn't find GAMS library :("));
  }
}

void FGamsLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGamsLibraryModule, GamsLibrary)
