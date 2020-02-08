// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GamsLibrary.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"


#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4103)
#pragma warning(disable:4191)
#pragma warning(disable:4457)
#pragma warning(disable:4458)
#pragma warning(disable:4459)
#pragma warning(disable:4530)
#pragma warning(disable:4577)
#pragma warning(disable:4583)
#pragma warning(disable:4582)
#pragma warning(disable:4668)
#pragma warning(disable:4996)
#include "madara/knowledge/KnowledgeBase.h"
#include "gams/controllers/Multicontroller.h"
#pragma warning(pop)

#define LOCTEXT_NAMESPACE "FGamsLibraryModule"

void FGamsLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

  // Get the base directory of this plugin
  const FString BaseDir = IPluginManager::Get ().FindPlugin (
    "GamsLibrary")->GetBaseDir ();
  const FString GamsDir = FPaths::Combine (
    *BaseDir, TEXT ("ThirdParty"), TEXT ("gams"));

  const FString MadaraBaseDir = IPluginManager::Get ().FindPlugin (
    "MadaraLibrary")->GetBaseDir ();
  const FString MadaraDir = FPaths::Combine (
    *BaseDir, TEXT ("ThirdParty"), TEXT ("madara"));

  // Add on the relative location of the third party dll and load it
  FString GamsLibraryPath;
  FString MadaraLibraryPath;
#if PLATFORM_WINDOWS
  GamsLibraryPath = FPaths::Combine (*GamsDir, TEXT ("Win64"), TEXT ("GAMS.dll"));
  MadaraLibraryPath = FPaths::Combine (*MadaraDir, TEXT ("Win64"), TEXT ("MADARA.dll"));
#elif PLATFORM_MAC
  GamsLibraryPath = FPaths::Combine (*GamsDir, TEXT ("Mac"), TEXT ("GAMS.dylib"));
  MadaraLibraryPath = FPaths::Combine (*MadaraDir, TEXT ("Mac"), TEXT ("MADARA.dylib"));
#endif // PLATFORM_WINDOWS

  GamsHandle = !GamsLibraryPath.IsEmpty() ?
    FPlatformProcess::GetDllHandle(*GamsLibraryPath) : nullptr;

  MadaraHandle = !MadaraLibraryPath.IsEmpty () ?
    FPlatformProcess::GetDllHandle (*MadaraLibraryPath) : nullptr;

  if (GamsHandle)
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("GAMS LOAD SUCCESS", "Found GAMS library"));
  }
  else
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("GAMS LOAD FAILURE", "Couldn't find GAMS library :("));
  }

  if (MadaraHandle)
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("MADARA LOAD SUCCESS", "Found MADARA library"));
  }
  else
  {
    FMessageDialog::Open (EAppMsgType::Ok,
      LOCTEXT ("MADARA LOAD FAILURE", "Couldn't find MADARA library :("));
  }

  madara::knowledge::KnowledgeBase kb;
  gams::controllers::Multicontroller controller (5);
  size_t num_check = controller.get_num_controllers ();

  std::stringstream buffer;
  buffer << "Multicontroller init test\ncontrollers loaded: ";
  buffer << num_check;
  FString status_text_f (buffer.str().c_str ());

  FMessageDialog::Open (EAppMsgType::Ok,
    FText::FromString (status_text_f));
}

void FGamsLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(GamsHandle);
  GamsHandle = nullptr;

  FPlatformProcess::FreeDllHandle (MadaraHandle);
  MadaraHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGamsLibraryModule, GamsLibrary)
