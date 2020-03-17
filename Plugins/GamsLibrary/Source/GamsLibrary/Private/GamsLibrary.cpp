// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GamsLibrary.h"
#include "GamsIncludes.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FGamsLibraryModule"

void FGamsLibraryModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

  // Get the base directory of this plugin
  const FString BaseDir = IPluginManager::Get().FindPlugin (
    "GamsLibrary")->GetBaseDir ();
  const FString GamsDir = FPaths::Combine(
    *BaseDir, TEXT("ThirdParty"), TEXT("gams"));

  const FString MadaraBaseDir = IPluginManager::Get ().FindPlugin (
    "MadaraLibrary")->GetBaseDir ();
  const FString MadaraDir = FPaths::Combine(
    *MadaraBaseDir, TEXT("ThirdParty"), TEXT("madara"));

  // Add on the relative location of the third party dll and load it
  FString EditorGamsLibraryPath;
  FString EditorMadaraLibraryPath;
  FString GameGamsLibraryPath;
  FString GameMadaraLibraryPath;
#if PLATFORM_WINDOWS
  // PIE causes the DLL copy function to stall if the editor is holding the
  // dll, so bind to the source copy of the DLL and not the destination
  EditorGamsLibraryPath = FPaths::Combine (
    *GamsDir, TEXT ("Win64"), TEXT ("GAMS.dll"));
  EditorMadaraLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Win64"), TEXT ("MADARA.dll"));
  GameMadaraLibraryPath = TEXT ("MADARA.dll");
  GameGamsLibraryPath = TEXT ("GAMS.dll");
#elif PLATFORM_MAC
  EditorGamsLibraryPath = FPaths::Combine (
    *GamsDir, TEXT ("Mac"), TEXT ("libGAMS.so"));
  EditorMadaraLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Mac"), TEXT ("libMADARA.so"));
  GameMadaraLibraryPath = TEXT ("libMADARA.so");
  GameGamsLibraryPath = TEXT ("libGAMS.so");
#elif PLATFORM_LINUX
  EditorGamsLibraryPath = FPaths::Combine (
    *GamsDir, TEXT ("Linux"), TEXT ("libGAMS.so"));
  EditorMadaraLibraryPath = FPaths::Combine (
    *MadaraDir, TEXT ("Linux"), TEXT ("libMADARA.so"));
  GameMadaraLibraryPath = TEXT ("libMADARA.so");
  GameGamsLibraryPath = TEXT ("libGAMS.so");
#endif // PLATFORM_WINDOWS

  // try to load editor source lib first to avoid dll copy issues
  GamsLibraryHandle = FPlatformProcess::GetDllHandle(*EditorGamsLibraryPath);

  if (!GamsLibraryHandle)
  {
    // try game lib if edit mode fails
    GamsLibraryHandle = FPlatformProcess::GetDllHandle(*GameGamsLibraryPath);

    if (!GamsLibraryHandle)
    {
      FMessageDialog::Open (EAppMsgType::Ok,
        LOCTEXT ("GAMS LOAD FAILURE",
          "GamsLibrary: Couldn't find GAMS library :("));
    }
  }

  // try to load editor source lib first to avoid dll copy issues
  MadaraLibraryHandle = FPlatformProcess::GetDllHandle (
    *EditorMadaraLibraryPath);

  if (!MadaraLibraryHandle)
  {
    // try game lib if edit mode fails
    MadaraLibraryHandle = FPlatformProcess::GetDllHandle(
      *GameMadaraLibraryPath);

    if (!MadaraLibraryHandle)
    {
      FMessageDialog::Open (EAppMsgType::Ok,
        LOCTEXT ("GAMS LOAD FAILURE",
          "GamsLibrary: Couldn't find MADARA library :("));
    }
  }

}

void FGamsLibraryModule::ShutdownModule()
{
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.

  // Free the dll handle
  FPlatformProcess::FreeDllHandle(GamsLibraryHandle);
  GamsLibraryHandle = nullptr;

  FPlatformProcess::FreeDllHandle(MadaraLibraryHandle);
  MadaraLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
  
IMPLEMENT_MODULE(FGamsLibraryModule, GamsLibrary)
