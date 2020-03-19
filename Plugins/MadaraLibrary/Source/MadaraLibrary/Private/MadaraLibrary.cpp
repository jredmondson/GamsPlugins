// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MadaraLibrary.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "MadaraIncludes.h"

#define LOCTEXT_NAMESPACE "FMadaraLibraryModule"

void FMadaraLibraryModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

  // Get the base directory of this plugin
  const FString BaseDir = IPluginManager::Get().FindPlugin(
    "MadaraLibrary")->GetBaseDir();
  const FString MadaraDir = FPaths::Combine(
    *BaseDir, TEXT("ThirdParty"), TEXT("madara"));

  // Add on the relative location of the third party dll and load it
  FString EditorLibraryPath;
  FString GameLibraryPath;
#if PLATFORM_WINDOWS
  EditorLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Win64"), TEXT ("MADARA.dll"));
  GameLibraryPath = TEXT ("MADARA.dll");
#elif PLATFORM_MAC
  EditorLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Mac"), TEXT ("libMADARA.so"));
  GameLibraryPath = TEXT ("libMADARA.so");
#elif PLATFORM_LINUX
  EditorLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Linux"), TEXT ("libMADARA.so"));
  GameLibraryPath = TEXT ("libMADARA.so");
#endif // PLATFORM_WINDOWS

  MadaraLibraryHandle = FPlatformProcess::GetDllHandle(*EditorLibraryPath);

  if (!MadaraLibraryHandle)
  {
    MadaraLibraryHandle = FPlatformProcess::GetDllHandle (*GameLibraryPath);
    if (!MadaraLibraryHandle)
    {
      FMessageDialog::Open (EAppMsgType::Ok,
        LOCTEXT ("MADARA LOAD FAILURE",
          "MadaraLibrary: Couldn't find MADARA library :("));
    }
  }
}

void FMadaraLibraryModule::ShutdownModule()
{
  // Free the dll handle
  FPlatformProcess::FreeDllHandle(MadaraLibraryHandle);
  MadaraLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
  
IMPLEMENT_MODULE(FMadaraLibraryModule, MadaraLibrary)
